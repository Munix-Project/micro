/*
 * term.c
 *
 *  Created on: Aug 30, 2015
 *      Author: miguel
 */
#include "term.h"
#include <termios.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <curses.h>
#include <sys/ioctl.h>
#include <string.h>
#include <unistd.h>

term_t thisterm;

void update_cursor() {
	getyx(curscr, thisterm.cur.y, thisterm.cur.x);
}

void clear_screen() {
	printf("\e[0;0H\e[2J");
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
	update_cursor();
	return c;
}

Point get_term_size() {
	Point size;
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);
	size.x = w.ws_col;
	size.y = w.ws_row;
	return size;
}

term_t init_term() {
	initscr();
	noecho();
	timeout(0);

	thisterm.cur.x = 0;
	thisterm.cur.y = 0;
	thisterm.size = get_term_size();

	thisterm.read = readkey;
	thisterm.clear = clear_screen;

	return thisterm;
}

void clean_term(term_t t) {

	endwin();
}
