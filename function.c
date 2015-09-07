/*
 * function.c
 *
 *  Created on: Sep 6, 2015
 *      Author: miguel
 */

#include "function.h"
#include "render.h"
#include "key.h"
#include "buffer.h"
#include "term.h"
#include <stdlib.h>

term_t * term;
status_t * status;

void init_func(file_t * file) {
	file->term_status = malloc(sizeof(status_t));
	status = file->term_status;
	status->quit_fl = 0;
	term = file->term;
}

void mod_quit() {
	status->quit_fl = 1;
}

/* Editor Functions Definitions: */

int f_exit() {
	mod_quit();
	return 0;
}

int f_copy() {

	return 0;
}

int f_cut() {

	return 0;
}

int f_paste() {

	return 0;
}

int f_find() {

	return 0;
}

int f_goto() {

	return 0;
}

int f_help() {

	return 0;
}

int f_more() {

	return 0;
}

int f_new() {

	return 0;
}

int f_open() {

	return 0;
}

int f_nextpage() {

	return 0;
}

int f_prevpage() {

	return 0;
}

int f_run() {

	return 0;
}

int f_save() {

	return 0;
}

int f_saveas() {

	return 0;
}

int f_undo() {

	return 0;
}

int execute_modifier(int mod) {
	int success = 1;

	switch(mod) {
	case F_EXIT:
		success = f_exit();
		break;
	case F_COPY:
		success = f_copy();
		break;
	case F_CUT:
		success = f_cut();
		break;
	case F_PASTE:
		success = f_paste();
		break;
	case F_FIND:
		success = f_find();
		break;
	case F_GOTO:
		success = f_goto();
		break;
	case F_HELP:
		success = f_help();
		break;
	case F_MORE:
		success = f_more();
		break;
	case F_NEW:
		success = f_new();
		break;
	case F_OPEN:
		success = f_open();
		break;
	case F_NEXTPAGE:
		success = f_nextpage();
		break;
	case F_PREVPAGE:
		success = f_prevpage();
		break;
	case F_RUN:
		success = f_run();
		break;
	case F_SAVE:
		success = f_save();
		break;
	case F_SAVEAS:
		success = f_saveas();
		break;
	case F_UNDO:
		success = f_undo();
		break;
	default:
		success = 0; /* Don't want the program to close for any other key */
		break;
	}
	return success;
}
