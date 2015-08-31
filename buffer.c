/*
 * buffer.c
 *
 *  Created on: Aug 31, 2015
 *      Author: miguel
 */

#include "buffer.h"
#include "key.h"
#include <stdlib.h>
#include <stdio.h>

mbuff_t micro_buff;

void create_line() {
	list_t * l = list_create();
	list_insert(micro_buff.buff, l);
	list_insert(l, '\r');
}

void init_buff() {
	micro_buff.buff = list_create();
	create_line();
}

void clean_buff() {
	foreach(line, micro_buff.buff) {
		list_free(line->value);
		free(line->value);
	}
	list_free(micro_buff.buff);
	free(micro_buff.buff);
}

void push_buff(Point cursorPos, int c) {
	/* push char into micro_buff on a certain location */
	node_t * rownode = list_get(micro_buff.buff, cursorPos.y);
	node_t * node_char = list_get(rownode->value, cursorPos.x);
	if(node_char)
		list_insert_before(rownode->value, node_char, (void*)c);
	else
		list_insert(rownode->value, (void*)c);
	if(c == K_NEWLINE) create_line();
}
