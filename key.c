/*
 * key.c
 *
 *  Created on: Aug 30, 2015
 *      Author: miguel
 */
#include "key.h"
#include "buffer.h"
#include "render.h"
#include <stdio.h>
#include <curses.h>
#include <stdint.h>

#define IGNORE -1
#define UP 0
#define DOWN 1
#define LEFT 0
#define RIGHT 1

#define VMOVE(DIR) term->update(DIR, IGNORE);
#define HMOVE(DIR) term->update(IGNORE, DIR);

Point old;
#define GOTO(x,y) old = term->go_to(x, y)
#define GOTOSTART() term->cur.x = 0;
#define RETCUR() term->go_to(old.x, old.y)

uint8_t is_esc = 0;

int consume_escape(int ch) {
	if(ch == K_ESC){
		is_esc = 1;
		getch(); /* consume '[' */
		return getch();
	} else
		return ch;
}

void handle_escape(term_t * term, int escode) {
	Point old_cur = term->cur;

	switch(escode) {
	case K_UP:
		VMOVE(UP);
		break;
	case K_DOWN:
		VMOVE(DOWN);
		break;
	case K_RIGHT:
		HMOVE(RIGHT);
		break;
	case K_LEFT:
		HMOVE(LEFT);
		break;
	}

	update_cursor_visual(term, old_cur);
	is_esc = 0;
}

void handle_normal(term_t * term, int c) {
	Point old_cur = term->cur;

	switch(c) {
	case K_NEWLINE:
		VMOVE(DOWN);
		GOTOSTART();
		break;
	case K_BACKSPACE:
		HMOVE(LEFT);
		break;
	default:
		HMOVE(RIGHT);
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
