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
		VMOVE(UP);
		GOTOLAST();
		break;
	case K_DOWN:
		VMOVE(DOWN);
		GOTOLAST();
		break;
	case K_RIGHT: {
		node_t * next_col = thiscol(term, thisrow(term));
		if(next_col && next_col->value != '\r' && next_col->value !='\n')
			HMOVE(RIGHT);
		else
			OVERFLOW();
		break;
	}
	case K_LEFT: {
		UNDERFLOW();
		HMOVE(LEFT);
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
		VMOVE(DOWN);
		GOTOFIRST();
		break;
	case K_BACKSPACE:
		HMOVE(LEFT);
		goto NOPUSH;
		break;
	default:
		HMOVE(RIGHT);
		break;
	}

	push_buff(old_cur, c);
	NOPUSH:
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
