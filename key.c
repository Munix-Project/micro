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
#include <stdio.h>
#include <curses.h>
#include <stdint.h>

uint8_t is_esc = 0;

int consume_escape(int ch) {
	if(ch == K_ESC){
		is_esc = 1;
		getch(); /* consume '[' */
		return getch();
	} else {
		return ch;
	}
}

void handle_escape(term_t * term, int escode) {
	Point old_cur = term->cur;

	switch(escode) {
	case K_UP:
		/* be REALLY careful with this macro */
		VSCROLL() }
		else { /* there's a hidden if inside hscroll, tricky one! */
			VMOVE(UP);
			GOTOLAST();
		}
		break;
	case K_DOWN: {
		/* Prevent the cursor from scrolling out of the "grid" */
		node_t * nextline = thisrow(term)->next;
		if(term->cur.y > term->size.y - (BOTTOM_MARGIN + 2) && nextline && list_size(nextline->value)) {
			render_y_off += DELTA_BOTTOM_SCROLL;
		} else {
			VMOVE(DOWN);
			GOTOLAST();
		}
		}
		break;
	case K_RIGHT: {
		if(term->cur.x > term->size.x - RIGHT_MARGIN) {
			render_x_off+=DELTA_RIGHT_SCROLL;
			HMOVEN(LEFT, DELTA_RIGHT_SCROLL);
			HMOVE(RIGHT); /* damn cursor won't go to its place */
		} else {
			node_t * next_col = thiscol(term, thisrow(term));
			if(next_col && next_col->value != K_NEWLINE)
				HMOVE(RIGHT);
			else {
				/* Scroll down here */
				node_t * nextline = thisrow(term)->next;
				if(term->cur.y > term->size.y - (BOTTOM_MARGIN + 2) && nextline && list_size(nextline->value)) {
					render_y_off += DELTA_BOTTOM_SCROLL;
					GOTOFIRST();
				} else {
					OVERFLOW();
				}
			}
		}
		break;
	}
	case K_LEFT: {
		if(!term->cur.x) {
			/* be REALLY careful with this macro */
			VSCROLL()
				while(1)
					if(!thiscol(term, thisrow(term)))
						break;
					else
						HMOVE(RIGHT);
			} else {
				UNDERFLOW();
			}
		}

		/* be REALLY careful with this macro */
		HSCROLL()
			/* Put cursor on the last character on the screen (but not on buffer) */
			HSCROLL_FINDLAST_ON_VIEW();
		} else { /* there's an hidden if inside hscroll, tricky one! */
			HMOVE(LEFT);
		}
		break;
	}
	}

	update_cursor_visual(term, old_cur);
	is_esc = 0;
}

void handle_normal(term_t * term, int c) {
	Point old_cur = term->cur;

	switch(c) {
	case K_NEWLINE:
		if(term->cur.y > term->size.y - (BOTTOM_MARGIN + 2))
			render_y_off+=DELTA_BOTTOM_SCROLL;
		else
			VMOVE(DOWN);
		GOTOFIRST();
		break;
	case K_BACKSPACE:
		if(!term->cur.x) {
			/* be REALLY careful with this macro */
			VSCROLL()
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

			}else {
				UNDERFLOW();
			}
		}
		/* be REALLY careful with this macro */
		HSCROLL()
			/* Put cursor on the last character on the screen (but not on buffer) */
			HSCROLL_FINDLAST_ON_VIEW();
		} else { /* there's a hidden if inside hscroll, tricky one! */
			HMOVE(LEFT);
		}
		break;
	case K_DEL: /* The cursor shall not move */ break;
	default:
		/* Normal character */
		if(term->cur.x > term->size.x - RIGHT_MARGIN) {
			render_x_off+=DELTA_RIGHT_SCROLL;
			HMOVEN(LEFT, DELTA_RIGHT_SCROLL - 1);
		} else {
			HMOVE(RIGHT);
		}
		break;
	}

	push_buff(old_cur, c);
	update_cursor_visual(term, old_cur);
}

void key_handle(term_t * term) {
	int c;
	if((c = term->read()) == TERM_NO_AVAIL) /* No key on the keyboard was hit */
		return;

	int c_cpy = consume_escape(c);
	if(is_esc)
		handle_escape(term, c_cpy);
	else
		handle_normal(term, c_cpy);
}
