/*
 * modal.c
 *
 *  Created on: Sep 7, 2015
 *      Author: miguel
 */

#include "defs.h"
#include "window.h"

void show_modal(file_t * file, char * header, char * body, char * footer, int modal_type) {
	/* Switch buffer input on key handling */
	file->is_modal = 1;
}

void close_modal(file_t * file) {
	set_window_border_defaults();
	file->is_modal = 0;
}
