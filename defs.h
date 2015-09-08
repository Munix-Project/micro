/*
 * defs.h
 *
 *  Created on: Aug 31, 2015
 *      Author: miguel
 */

#ifndef DEFS_H_
#define DEFS_H_

#include "function.h"
#include "window.h"
#include "list.h"
#include "term.h"

#define MICRO_NAME "Munix Micro"
#define NEW_FILE "New-File"

typedef struct {
	term_t * term;
	status_t * term_status;
	render_t * rend;
	list_t * buff;
	list_t * modbuff;
	uint8_t is_modal;
	char * filename;
	char * path;
	int size;
} file_t;

extern void init_func(file_t * t);

extern int execute_modifier(int mod);
extern void mod_quit();

extern file_t * create_file(char * where, char * filename);
extern void close_file(file_t * file);

#endif /* DEFS_H_ */
