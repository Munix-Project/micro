/*
 * micro.c
 *
 *  Created on: Aug 30, 2015
 *      Author: miguel
 *
 *  A text editor for the Helios Operating System
 */

#include "micro_file.h"
#include "render.h"
#include "key.h"

int main(int argc, char ** argv) {
	file_t * file = create_file(".", "new-file");

	while(!file->term_status->quit_fl) {
		key_handle(file);
		render_all(file);
	}

	close_file(file);
	return 0;
}
