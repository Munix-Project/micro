/*
 * buffer.c
 *
 *  Created on: Aug 31, 2015
 *      Author: miguel
 */

#include "buffer.h"
#include "key.h"
#include "window.h"
#include "render.h"
#include <stdlib.h>
#include <stdio.h>

list_t * micro_buff;

list_t * create_line(int y) {
	list_t * l = list_create();

	if(y > list_size(micro_buff)) {
		/* Create new line */
		list_insert(micro_buff, l);
	} else {
		node_t * nextline = list_get(micro_buff, y);
		list_insert_before(micro_buff, nextline, l);
	}
	return l;
}

void init_buff() {
	micro_buff = list_create();
	list_t * firstline = create_line(0);
	list_insert(firstline, K_NEWLINE);
}

void clean_buff() {
	foreach(line, micro_buff) {
		list_free(line->value);
		free(line->value);
	}
	list_free(micro_buff);
	free(micro_buff);
}

node_t * thisrow(term_t * term) {
	return list_get(micro_buff, render_y_off + (term->cur.y - TOP_MARGIN));
}

node_t * thiscol(term_t * term, node_t * row) {
	return list_get(row->value, render_x_off + term->cur.x);
}

void remove_from_pos_until_empty(list_t* line, int index) {
	for(int i=0;;i++) {
		if(!list_get(line,index)) break;
		list_remove(line, index);
	}
}

void func_buff(Point cursorPos, node_t * row, node_t * col, int c) {
	switch(c) {
	case K_BACKSPACE:
		if(!cursorPos.x && row->prev) {
			/* Copy this line, append it to the previous one and delete this line */
			node_t * prev_newline = list_find(row->prev->value, K_NEWLINE);
			forl(int i=0, 1, 1, (list_t*)row->value)
				list_insert_before(row->prev->value, prev_newline, node->value);

			remove_from_pos_until_empty(row->value, 0);
			list_remove(micro_buff, render_y_off +  (cursorPos.y - TOP_MARGIN));
		} else {
			list_remove(row->value, render_x_off + cursorPos.x - 1);
		}
		break;
	case K_DEL:
		/* Nothing else to delete, please don't corrupt my memory */
		if(!row->next && col && col->value == K_NEWLINE) break;

		if(col->value == K_NEWLINE) {
			/* Remove new line, bring the next line to this one and get rid of the next line */
			forl(int i=0, 1, 1, (list_t*)row->next->value)
				list_insert_before(row->value, col, node->value);
			remove_from_pos_until_empty(row->next->value, 0);
			list_remove(micro_buff, render_y_off + ((cursorPos.y + 1) - TOP_MARGIN));
		} else if(list_get_last(row->value) == col){
			/* Don't delete \r if that's the only thing that is on this line (this means there's no \n to delete) */
		} else {
			/* Delete regular character */
			list_remove(row->value, render_x_off + cursorPos.x);
		}
		break;
	}
}

void push_buff(Point cursorPos, int c) {
	/* push char into micro_buff on a certain location */
	int row_y = render_y_off + (cursorPos.y - TOP_MARGIN);
	int row_x = render_x_off + cursorPos.x;
	node_t * rownode = list_get(micro_buff, row_y);
	node_t * node_char = list_get(rownode->value, row_x);

	if(c == K_BACKSPACE || c == K_DEL) {
		/* A special character has been pushed into the buffer.
		 * Normally, this character should not be actually pushed.
		 * Instead, it affects the contents of the buffer. */
		func_buff(cursorPos, rownode, node_char, c);
		return;
	}

	if(node_char)
		list_insert_before(rownode->value, node_char, (void*)c);
	else
		list_insert_before(rownode->value, list_find(rownode->value, K_NEWLINE), (void*)c);

	if(c == K_NEWLINE || !rownode->next) {
		/* Check if we want to insert or create a new line */
		list_t * newline = create_line(row_y + 1);
		list_t * thisline = rownode->value;

		/* Move everything after \n to the next line */
		if(c == K_NEWLINE) { /* Don't want to move stuff nore remove if we found a null line! */
			forl(int i = list_index_of(thisline, K_NEWLINE) + 1, 1, 1, thisline)
				list_insert(newline, node->value);

			/* Remove old characters from previous line */
			int newpos = list_index_of(thisline, K_NEWLINE) + 1;
			remove_from_pos_until_empty(thisline, newpos);
		}
	}
}
