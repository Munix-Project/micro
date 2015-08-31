/*
 * cursor.h
 *
 *  Created on: Aug 31, 2015
 *      Author: miguel
 */

#ifndef CURSOR_H_
#define CURSOR_H_

/******* Cursor mechanisms *******/

#define IGNORE -1
#define UP 0
#define DOWN 1
#define LEFT 0
#define RIGHT 1

/* Handles */
#define VMOVE(DIR) term->update(DIR, IGNORE);
#define HMOVE(DIR) term->update(IGNORE, DIR);

Point old;
#define GOTO(x,y) old = term->go_to(x, y)
#define RETCUR() term->go_to(old.x, old.y)
#define GOTOFIRST() term->cur.x = 0;
#define GOTOLAST() { node_t * row = list_get(micro_buff.buff, term->cur.y); \
					if(row && is_loc_void(term->cur) && list_size(row->value)) \
						term->cur.x = list_index_of_node(row->value, list_get_last(row->value)) + 1; }
#define UNDERFLOW() {if(!term->cur.x && term->cur.y) { \
						term->cur.y--; \
						node_t * row = list_get(micro_buff.buff, term->cur.y); \
						term->cur.x = list_index_of_node(row->value, list_get_last(row->value)) + 1; \
					}}
#define OVERFLOW() 	{node_t * nextrow = list_get(micro_buff.buff, term->cur.y + 1); \
					node_t * thisrow = list_get(micro_buff.buff, term->cur.y); \
					if(nextrow && term->cur.x == list_index_of_node(thisrow->value, list_get_last(thisrow->value)) + 1) { \
						term->cur.y++; \
						term->cur.x = 0; \
					}}

#endif /* CURSOR_H_ */
