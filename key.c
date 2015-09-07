/*
 * key.c
 *
 *  Created on: Aug 30, 2015
 *      Author: miguel
 */
#include "window.h"
#include "key.h"
#include "micro_file.h"
#include "term.h"
#include "buffer.h"
#include "render.h"
#include "function.h"
#include <stdint.h>

#define CTRL_START 1
#define CTRL_END CTRL_START + ('Z' - 'A')

#define NEEDS_SCROLL_RIGHT() file->term->cur.x > file->term->size.x - RIGHT_MARGIN
#define NEEDS_SCROLL_DOWN() file->term->cur.y > file->term->size.y - (BOTTOM_MARGIN + 2) && nextline && list_size(nextline->value)
#define IS_CURSOR_ON_START() !(file->term->cur.x - LEFT_MARGIN)

#define FALLBACK_START 1
#define FALLBACK_LAST  2
#define FALLFORW 3

uint8_t fallback = 0;

uint8_t is_esc = 0;
uint8_t is_ctrl = 0;

void cursor_scroll_right(file_t * file) {
	/* Scroll right */
	file->rend->x_off+=DELTA_RIGHT_SCROLL;

	HMOVEN(LEFT, DELTA_RIGHT_SCROLL);
	HMOVE(RIGHT); /* damn cursor won't go to its place */
}

void cursor_scroll_down(file_t * file) {
	file->rend->y_off+=DELTA_BOTTOM_SCROLL;
}

void cursor_scroll_left(file_t * file) {
	file->rend->x_off-=DELTA_LEFT_SCROLL;
	if(file->rend->x_off < 0)
		file->rend->x_off = 0;
}

void cursor_scroll_up(file_t * file) {
	file->rend->y_off-=DELTA_TOP_SCROLL;
	if(file->rend->y_off < 0)
		file->rend->y_off = 0;
}

void fall_back_forward(file_t * file) {
	/* Fall back the cursor from a scrolled position. This might also fall "forward", not just back.
	 * This needs to be done here after pushing the buffer
	 * because we're changing the render offsets and we can't change
	 * them BEFORE we push the character into the buffer.
	 * If we did indeed push them, the index on the buffer
	 * would be totally wrong and all kinds of problems would show up. */
	switch(fallback) {
	case FALLBACK_START:
		while(file->rend->x_off)
			cursor_scroll_left(file);
		break;
	case FALLBACK_LAST: {
		node_t * row = thisrow(file);
		if(!row) break; /* This row doesn't exist */
		if(is_loc_void(file, file->term->cur)) {
			/* Check if we really need to scroll left */
			int line_window_cover = (int)(list_size(row->value) / (file->term->size.x - LEFT_MARGIN));
			if((int)(file->rend->x_off / DELTA_RIGHT_SCROLL) > line_window_cover) {
				/* We need: */
				/* Scroll left till something is found */
				while(is_loc_void(file, file->term->cur) && file->rend->x_off)
					cursor_scroll_left(file);
			}
			/* Find the very last column on the same line */
			GOTOLAST();
			HSCROLL_FINDLAST_ON_VIEW();
		} /* else: Already on a good place */
		break;
	}
	case FALLFORW: {
		if(NEEDS_SCROLL_RIGHT()) {
			while(NEEDS_SCROLL_RIGHT()) {
				cursor_scroll_right(file);
				Point loc = file->term->cur;
				loc.x = file->rend->x_off + (loc.x - LEFT_MARGIN);
				if(is_loc_void(file, loc)) break;
			}
			HSCROLL_FINDLAST_ON_VIEW();
		}
		break;
	}
	}
	fallback = 0;
}

void handle_escape(file_t * file, int escode) {
	Point old_cur = file->term->cur;

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
		node_t * nextline = thisrow(file)->next;
		if(NEEDS_SCROLL_DOWN()) {
			cursor_scroll_down(file); /* Scroll down */
		} else {
			/* Move cursor down */
			VMOVE(DOWN);
			fallback = FALLBACK_LAST;
		}
		}
		break;
	case K_RIGHT: {
		if(NEEDS_SCROLL_RIGHT()) {
			/* Check if there is anything else forward that we can scroll into */
			if(nextcol(file, thisrow(file)))
				cursor_scroll_right(file); /* Scroll right */
		} else {
			/* Move cursor to the right */
			node_t * this_row = thisrow(file);
			node_t * next_col = thiscol(file, this_row);
			if(next_col && next_col->value != K_NEWLINE) {
				HMOVE(RIGHT);
			} else {
				/* Scroll down here */
				node_t * nextline = this_row->next;
				if(NEEDS_SCROLL_DOWN()) {
					cursor_scroll_down(file);
					GOTOFIRST();
				} else {
					/* Fall back to the start of the document */
					if(nextrow(file))
						fallback = FALLBACK_START;

					/* Overflow cursor to the next line */
					OVERFLOW();
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

	fall_back_forward(file);
	/* Update position only if cursor changes are valid */
	update_cursor_visual(file, old_cur);
	is_esc = 0; /* The escape function has been handled */
}

void handle_normal(file_t * file, int c) {
	Point old_cur = file->term->cur;

	switch(c) {
	case K_NEWLINE:
		if(file->term->cur.y > file->term->size.y - (BOTTOM_MARGIN + 2))
			cursor_scroll_down(file); /* Scroll down */
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
				file->rend->y_off -= DELTA_TOP_OFF_PLUS;
				if(file->rend->y_off < 0) file->rend->y_off = 0;

				GOTOFIRST();
				for(int i=0;i<DELTA_TOP_OFF_PLUS;i++) {
					if(!thisrow(file)->next)
						break;
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
			cursor_scroll_right(file); /* Scroll right */
		else
			HMOVE(RIGHT); /* Move cursor to the right */
		break;
	}

	/* Push character entered into the text editor buffer */
	push_buff(file, old_cur, c);
	fall_back_forward(file);
	/* Update position only if new cursor changes are valid */
	update_cursor_visual(file, old_cur);
}

void handle_modifier(int c) {
	if(execute_modifier(c)) {
		mod_quit(); /* The modifier function failed. Send command (set the flag) to close the program */
		/* TODO: In the future try to handle the error (also display it to the user) */
	}
	is_ctrl = 0;
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

int consume_ctrl(int c) {
	/* There's a big problem here. Newline is 10 in ascii. If we
	 * consider it as a modifier then we're in BIG trouble.
	 * We must make an exception for the newline character, so
	 * we're going to put it on the if statement. */
	if(c >= CTRL_START && c <= CTRL_END && c != '\n') {
		is_ctrl = 1;
		return c + 'A' - 1;
	} else {
		return c;
	}
}

int kbhit(term_t * term) {
	int c;
	if((c = term->read()) != TERM_NO_AVAIL)
		return c;
	else
		return 0;
}

int consume_input(int c) {
	/* Detect if a special key / combination of keys (like the arrows or ctrl + modifier) has / have been entered */
	return consume_ctrl(consume_escape(c));
}

void key_handle(file_t * file) {
	/* Read the keyboard without blocking the program */
	int c;
	if(!(c = kbhit(file->term)))
		return; /* No key on the keyboard was hit */

	int c_cpy = consume_input(c);
	if(is_esc)
		handle_escape(file, c_cpy);
	else if(is_ctrl)
		handle_modifier(c_cpy);
	else
		handle_normal(file, c_cpy);
}
