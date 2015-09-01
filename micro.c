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
#include "render.h"

#include "list.h"
#include <stdio.h>
int main(int argc, char ** argv) {
	term_t * term = init_term();
	term->clr();
	init_renderer();

	while(1) {
		key_handle(term);
		render_all(term);
	}

	clean_term(term);
	clean_renderer();
	return 0;
}
