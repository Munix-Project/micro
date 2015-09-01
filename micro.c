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
	/*list_t * l = list_create();
	list_insert(l, 1);
	list_insert(l, 2);
	list_insert(l, 3);

	list_remove(l,1);

	printf("%d\n", list_size(l));

	forl(int i=0,1,1,l){

		printf("%d, ", node->value);
	}

	fflush(stdout);

	for(;;);*/
	term_t * term = init_term();
	term->clear();
	init_renderer();

	while(1) {
		key_handle(term);
		render_all(term);
	}

	clean_term(term);
	clean_renderer();
	return 0;
}
