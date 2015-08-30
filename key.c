/*
 * key.c
 *
 *  Created on: Aug 30, 2015
 *      Author: miguel
 */
#include "key.h"
#include <stdio.h>
#include <curses.h>
#include <stdint.h>

/* keys that don't belong to ncurses */
enum KEYS{
	K_NEWLINE = 10,
	K_ESC = 27,
	K_UP = 'A',
	K_DOWN = 'B',
	K_LEFT = 'C',
	K_RIGHT = 'D'
};

int handle_escape(int ch) {
	if(ch == K_ESC){
		getch(); /* consume '[' */
		return getch();
	} else
		return ch;
}

void key_handle(term_t term) {
	int c;
	if((c = term.read()) == TERM_NO_AVAIL) /* No key on the keyboard was hit */
		return;

	switch(handle_escape(c)) {
	case K_UP: 		printf("\e[1A\r"); 	break;
	case K_DOWN: 	printf("\e[1B\r"); 	break;
	case K_LEFT: 	printf("\e[1C"); 	break;
	case K_RIGHT: 	printf("\e[1D"); 	break;
	case K_NEWLINE: printf("\n\r"); 	break;
	default: 		printf("%c", c); 	break;
	}
	fflush(stdout);
}
