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

node_t * create_line(int y) {
	list_t * l = list_create();

	if(y > list_size(micro_buff.buff)) {
		/* Create new line */
		list_insert(micro_buff.buff, l);
		list_insert(l, '\r');
	} else {
		node_t * nextline = list_get(micro_buff.buff, y);
		list_insert_before(micro_buff.buff, nextline, l);
		list_insert(l, '\r');
	}
	return l;
}

void init_buff() {
	micro_buff.buff = list_create();
	create_line(0);
}

void clean_buff() {
	foreach(line, micro_buff.buff) {
		list_free(line->value);
		free(line->value);
	}
	list_free(micro_buff.buff);
	free(micro_buff.buff);
}

node_t * thisrow(term_t * term) {
	return list_get(micro_buff.buff, term->cur.y);
}

node_t * thiscol(term_t * term, node_t * row) {
	return list_get(row->value, term->cur.x);
}

void push_buff(Point cursorPos, int c) {
	/* push char into micro_buff on a certain location */
	node_t * rownode = list_get(micro_buff.buff, cursorPos.y);
	node_t * node_char = list_get(rownode->value, cursorPos.x);
	if(node_char)
		list_insert_before(rownode->value, node_char, (void*)c);
	else
		list_insert(rownode->value, (void*)c);

	if(c == K_NEWLINE) {
		/* Check if we want to insert or create a new line */
		list_t * newline = create_line(cursorPos.y + 1);
		list_t * thisline = rownode->value;

		/* Move everything after \n to the next line */
		node_t * carriage_node = list_find(newline, '\r');
		forl(int i = list_index_of(thisline, '\n') + 1, 1, 1, thisline)
			list_insert_before(newline, carriage_node, node->value);

		/* Remove old characters from previous line */
		int newpos = list_index_of(thisline, '\n') + 1;
		for(int i=0;;i++) {
			if(!list_get(thisline,newpos)) break;
			list_remove(thisline, newpos);
		}
	}
}
