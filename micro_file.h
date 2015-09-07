/*
 * file.h
 *
 *  Created on: Sep 6, 2015
 *      Author: miguel
 */

#ifndef MICRO_FILE_H_
#define MICRO_FILE_H_

#include "function_def.h"
#include "window.h"
#include "list.h"
#include "term.h"

typedef struct {
	term_t * term;
	status_t * term_status;
	render_t * rend;
	list_t * buff;
	char * filename;
	char * path;
	int size;
} file_t;

extern file_t * create_file(char * where, char * filename);
extern void close_file(file_t * file);

#endif /* MICRO_FILE_H_ */
