/*
 * micro.c
 *
 *  Created on: Aug 30, 2015
 *      Author: miguel
 *
 *  A text editor for the Helios Operating System
 */

#include "term.h"
#include "key.h"
#include <stdio.h>

int main(int argc, char ** argv) {
	term_t term = init_term();
	term.clear();

	while(1) {
		key_handle(term);
	}

	clean_term(term);
	return 0;
}
