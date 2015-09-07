/*
 * term.c
 *
 *  Created on: Aug 30, 2015
 *      Author: miguel
 */
#include "term.h"
#include "window.h"
#include "function.h"
#include <termios.h>
#include <stdlib.h>
#include <ncurses.h>
#include <sys/ioctl.h>

term_t * thisterm;

Point get_term_size() {
	Point size;
	struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);
    size.x = w.ws_col;
	size.y = w.ws_row;
	return size;
}

void fix_resize() {
	if(thisterm->cur.x >= thisterm->size.x - 1)
		thisterm->cur.x = thisterm->size.x - 2;
	if(thisterm->cur.y >= thisterm->size.y - 1)
		thisterm->cur.y = thisterm->size.y - 2;
}

void update_term_size() {
	thisterm->size = get_term_size();
	fix_resize();
}

void update_cursor(uint8_t movedown, uint8_t moveright) {
	if(movedown == 1 && thisterm->cur.y < thisterm->size.y - 1)
		thisterm->cur.y++;
	else if(!movedown && thisterm->cur.y > 0)
		thisterm->cur.y--;

	if(moveright == 1 && thisterm->cur.x < thisterm->size.x - 1)
		thisterm->cur.x++;
	else if(!moveright && thisterm->cur.x > LEFT_MARGIN)
		thisterm->cur.x--;
}

void update_all() {
	update_term_size();
}

Point cursor_goto(int x, int y) {
	Point old;
	old.x = thisterm->cur.x;
	old.y = thisterm->cur.y;

	if(x >= thisterm->size.x)
		thisterm->cur.x = x = thisterm->size.x - 2;
	if(y >= thisterm->size.y)
		thisterm->cur.y = y = thisterm->size.y - 2;

	move(y, x);
	return old;
}

void clear_screen() {
	for(int y = 0; y < thisterm->size.y; y++)
		for(int x = 0; x < thisterm->size.x; x++) {
			move(y, x);
			addch(' ');
		}
	move(0, 0);
}

int getchne() {
	/*  getch non blocking with no echo */
	int ch = getch();
	if(ch > 0)
		return ch;
	else
		return -1;
}

int readkey() {
	int c = getchne();
	update_all();
	return c;
}

term_t * init_term() {
	initscr();
	noecho();
	raw();
	timeout(0);

	thisterm = malloc(sizeof(term_t));
	thisterm->cur.x = LEFT_MARGIN;
	thisterm->cur.y = TOP_MARGIN;
	update_all();

	thisterm->read = readkey;
	thisterm->clr = clear_screen;
	thisterm->go_to = cursor_goto;
	thisterm->update = update_cursor;

	return thisterm;
}

void clean_term(term_t * t) {
	free(t);
	endwin();
}
