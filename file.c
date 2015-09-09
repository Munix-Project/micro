/*
 * file.c
 *
 *  Created on: Sep 6, 2015
 *      Author: miguel
 */

#include "defs.h"
#include "render.h"
#include "buffer.h"
#include "key.h"
#include <stdlib.h>

file_t * create_file(char * where, char * filename) {
	file_t * file = malloc(sizeof(file_t));
	file->term = init_term();
	file->buff = init_buff();
	file->modbuff = init_buff();
	file->rend = init_renderer();
	file->path = where;
	file->filename = filename;
	file->size = 0;
	file->is_modal = 0;

	init_keys();
	init_func(file);
	return file;
}

void close_file(file_t * file) {
	clean_term(file->term);
	clean_renderer(file->rend);
	clean_buff(file->buff);
	clean_buff(file->modbuff);
	clean_keys();
}
