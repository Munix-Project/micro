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
	K_NEWLINE 	= 	10,
	K_ESC 		= 	27,
	K_UP 		= 	'A',
	K_DOWN 		= 	'B',
	K_LEFT 		= 	'C',
	K_RIGHT 	= 	'D'
};

uint8_t is_esc = 0;

int consume_escape(int ch) {
	if(ch == K_ESC){
		is_esc = 1;
		getch(); /* consume '[' */
		return getch();
	} else
		return ch;
}

void handle_escape(int escode) {
	switch(escode) {
	case K_UP: 		printf("\e[1A\r"); 	break;
	case K_DOWN: 	printf("\e[1B\r"); 	break;
	case K_LEFT: 	printf("\e[1C"); 	break;
	case K_RIGHT: 	printf("\e[1D"); 	break;
	}
	is_esc = 0;
}

void handle_normal(int c) {
	if(c == K_NEWLINE)
		printf("\r");
	printf("%c", c);
}

void key_handle(term_t term) {
	int c;
	if((c = term.read()) == TERM_NO_AVAIL) /* No key on the keyboard was hit */
		return;

	int c_cpy = consume_escape(c);
	if(is_esc)
		handle_escape(c_cpy);
	else
		handle_normal(c_cpy);
	fflush(stdout);
}
