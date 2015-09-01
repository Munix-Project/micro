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
#define VMOVE(DIR) term->update(DIR, IGNORE)
#define HMOVE(DIR) term->update(IGNORE, DIR)

Point old;
#define GOTO(x,y) old = term->go_to(x, y)
#define RETCUR() term->go_to(old.x, old.y)
#define GOTOFIRST() term->cur.x = 0;
#define GOTOLAST() { node_t * row = list_get(micro_buff.buff, term->cur.y - TOP_MARGIN); \
					 if(row && is_loc_void(term->cur) && list_size(row->value)) {\
						term->cur.x = list_index_of_node(row->value, list_find(row->value, K_CARRIAGE)); \
						if(is_loc_void(term->cur)) \
							term->cur.x = list_index_of_node(row->value, list_find(row->value, K_NEWLINE));\
							if(is_loc_void(term->cur)) \
								term->cur.x = 0; \
						}}
#define UNDERFLOW() {if(!term->cur.x && (term->cur.y - TOP_MARGIN)) { \
							node_t * prevrow = list_get(micro_buff.buff, (term->cur.y - TOP_MARGIN) - 1); \
							term->cur.y--; \
							term->cur.x = list_index_of_node(prevrow->value, list_find(prevrow->value, K_NEWLINE)) + 1; \
						}}
#define OVERFLOW(row) 	{node_t * nextrow = list_get(micro_buff.buff, (term->cur.y - TOP_MARGIN) + 1); \
						if(nextrow) { \
							term->cur.y++; \
							term->cur.x = 0; \
						}}

#endif /* CURSOR_H_ */
