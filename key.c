/*
 * key.c
 *
 *  Created on: Aug 30, 2015
 *      Author: miguel
 */
#include "key.h"
#include "buffer.h"
#include "render.h"
#include "cursor.h"
#include "window.h"
#include <stdint.h>

#define NEEDS_SCROLL_RIGHT() term->cur.x > term->size.x - RIGHT_MARGIN
#define NEEDS_SCROLL_DOWN() term->cur.y > term->size.y - (BOTTOM_MARGIN + 2) && nextline && list_size(nextline->value)
#define IS_CURSOR_ON_START() !term->cur.x

#define FALLBACK_START 1
#define FALLBACK_LAST  2
#define FALLFORW 3

uint8_t fallback = 0;

uint8_t is_esc = 0;

void cursor_scroll_right(term_t * term) {
	/* Scroll right */
	render_x_off+=DELTA_RIGHT_SCROLL;
	HMOVEN(LEFT, DELTA_RIGHT_SCROLL);
	HMOVE(RIGHT); /* damn cursor won't go to its place */
}

void cursor_scroll_down() {
	render_y_off+=DELTA_BOTTOM_SCROLL;
}

void cursor_scroll_left(term_t * term) {
	render_x_off-=DELTA_RIGHT_SCROLL;
}

void cursor_scroll_up() {
	render_y_off-=DELTA_TOP_SCROLL;
	if(render_y_off < 0)
		render_y_off = 0;
}

void fall_back_forward(term_t * term) {
	/* Fall back the cursor from a scrolled position. This might also fall "forward", not just back.
	 * This needs to be done here after pushing the buffer
	 * because we're changing the render offsets and we can't change
	 * them BEFORE we push the character into the buffer.
	 * If we did indeed push them, the index on the buffer
	 * would be totally wrong and all kinds of problems would show up. */
	switch(fallback) {
	case FALLBACK_START:
		while(render_x_off)
			cursor_scroll_left(term);
		break;
	case FALLBACK_LAST :
		if(!thisrow(term)) break; /* This row doesn't exist */
		if(is_loc_void(term->cur)) {
			/* Scroll left till something is found */
			while(is_loc_void(term->cur) && render_x_off)
				cursor_scroll_left(term);
			/* Find the very last column on the same line */
			GOTOLAST();
			HSCROLL_FINDLAST_ON_VIEW();
		} /* else: Already on a good place */
		break;
	case FALLFORW: {
		while(NEEDS_SCROLL_RIGHT()) {
			cursor_scroll_right(term);
			Point loc = term->cur;
			loc.x += render_y_off;
			if(is_loc_void(loc)) break;
		}
		HSCROLL_FINDLAST_ON_VIEW();
		break;
	}
	}
	fallback = 0;
}

void handle_escape(term_t * term, int escode) {
	Point old_cur = term->cur;

	switch(escode) {
	case K_UP:
		/* be REALLY careful with this macro */
		VSCROLL() } /* Scroll up */
		else { /* there's a hidden if inside hscroll, tricky one! */
			/* Move cursor up */
			VMOVE(UP);
			fallback = FALLBACK_LAST;
		}
		break;
	case K_DOWN: {
		/* Prevent the cursor from scrolling out of the "grid" */
		node_t * nextline = thisrow(term)->next;
		if(NEEDS_SCROLL_DOWN()) {
			cursor_scroll_down(); /* Scroll down */
		} else {
			/* Move cursor down */
			VMOVE(DOWN);
			fallback = FALLBACK_LAST;
		}
		}
		break;
	case K_RIGHT: {
		if(NEEDS_SCROLL_RIGHT()) {
			/* Scroll right */
			cursor_scroll_right(term);
		} else {
			/* Move cursor to the right */
			node_t * this_row = thisrow(term);
			node_t * next_col = thiscol(term, this_row);
			if(next_col && next_col->value != K_NEWLINE) {
				HMOVE(RIGHT);
			} else {
				/* Scroll down here */
				node_t * nextline = this_row->next;
				if(NEEDS_SCROLL_DOWN()) {
					cursor_scroll_down();
					GOTOFIRST();
				} else {
					/* Overflow cursor to the next line */
					OVERFLOW();
					/* Fall back to the start of the document */
					if(thisrow(term)->next)
						fallback = FALLBACK_START;
				}
			}
		}
		break;
	}
	case K_LEFT: {
		if(IS_CURSOR_ON_START()) {
			/* be REALLY careful with this macro */
			VSCROLL() /* Scroll up */
				FALL_FORWARD(); /* Finds the very very last character on the current line and scrolls to it */
			} else {
				/* Underflow cursor to the previous line */
				UNDERFLOW();
				if(under) /* Underflow happened */
					fallback = FALLFORW; /* Fall forward to the end of the line */
			}
		}

		/* be REALLY careful with this macro */
		HSCROLL() /* Scroll left (this only runs if necessary, else it runs the 'else') */
			/* Put cursor on the last character on the screen (but not on buffer) */
			HSCROLL_FINDLAST_ON_VIEW();
		} else { /* there's an hidden if inside hscroll, tricky one! */
			/* Move cursor to the left */
			HMOVE(LEFT);
		}
		break;
	}
	}

	fall_back_forward(term);
	/* Update position only if cursor changes are valid */
	update_cursor_visual(term, old_cur);
	is_esc = 0; /* The escape function has been handled */
}

void handle_normal(term_t * term, int c) {
	Point old_cur = term->cur;

	switch(c) {
	case K_NEWLINE:
		if(term->cur.y > term->size.y - (BOTTOM_MARGIN + 2))
			cursor_scroll_down(); /* Scroll down */
		else
			VMOVE(DOWN); /* Move the cursor down */
		GOTOFIRST();
		/* Fall back to the start of the document */
		fallback = FALLBACK_START;
		break;
	case K_BACKSPACE:
		if(IS_CURSOR_ON_START()) {
			/* be REALLY careful with this macro */
			VSCROLL() /* Scroll up */
				/* Backspace is kinda special. It scrolls down some extra lines because it's deleting content */
				int DELTA_TOP_OFF_PLUS = 10;
				render_y_off -= DELTA_TOP_OFF_PLUS;
				if(render_y_off<0) render_y_off = 0;

				GOTOFIRST();
				for(int i=0;i<DELTA_TOP_OFF_PLUS;i++) {
					if(!thisrow(term)->next){
						VMOVE(UP);
						break;
					}
					VMOVE(DOWN);
				}

				FALL_FORWARD(); /* Finds the very very last character on the current line and scrolls to it */
			} else {
				/* Underflow cursor to the previous line */
				UNDERFLOW();
				if(under) /* Underflow happened */
					fallback = FALLFORW;
			}
		}
		/* be REALLY careful with this macro */
		HSCROLL() /* Scroll left */
			/* Put cursor on the last character on the screen (but not on buffer) */
			HSCROLL_FINDLAST_ON_VIEW();
		} else { /* there's a hidden if inside hscroll, tricky one! */
			/* Move cursor to the left */
			HMOVE(LEFT);
		}
		break;
	case K_DEL: /* The cursor shall not move */ break;
	default:
		/* Normal character */
		if(NEEDS_SCROLL_RIGHT())
			cursor_scroll_right(term); /* Scroll right */
		else
			HMOVE(RIGHT); /* Move cursor to the right */
		break;
	}

	/* Push character entered into the text editor buffer */
	push_buff(old_cur, c);
	fall_back_forward(term);
	/* Update position only if new cursor changes are valid */
	update_cursor_visual(term, old_cur);
}

int consume_escape(int ch) {
	if(ch == K_ESC){
		is_esc = 1;
		getch(); /* consume '[' */
		return getch();
	} else {
		return ch;
	}
}

void key_handle(term_t * term) {
	/* Read the keyboard without blocking the program */
	int c;
	if((c = term->read()) == TERM_NO_AVAIL) /* No key on the keyboard was hit */
		return;

	/* Detect if a special key (like the arrows) has been entered */
	int c_cpy = consume_escape(c);
	if(is_esc)
		handle_escape(term, c_cpy);
	else
		handle_normal(term, c_cpy);
}
