/*
 * micro.c
 *
 *  Created on: Aug 30, 2015
 *      Author: miguel
 *
 *  A text editor for the Helios Operating System
 */

#include <termios.h>
#include <stdio.h>
#include <stdint.h>

struct termios old, new;

void clear() {
	printf("\e[0;0H\e[2J");
	fflush(stdout);
}

void set_echo(uint8_t disable) {
	tcgetattr(fileno(stdin), &old);
	new = old;
	if(disable) {
		new.c_lflag &= ~ECHO;
		new.c_lflag &= ~ICANON;
		tcsetattr(fileno(stdin), TCSAFLUSH, &new);
	}
	else {
		(void) tcsetattr (fileno (stdin), TCSAFLUSH, &old);
	}
}

void disable_echo() {
	set_echo(1);
}

void enable_echo() {
	set_echo(0);
}

char readkey() {
	disable_echo();
	char c = getc(stdin);
	enable_echo();
	return c;
}

int main(int argc, char ** argv) {
	clear();
	while(1){
		char c = readkey();
		switch(c) {
		default: putchar(c);
		}
	}
	return 0;
}
