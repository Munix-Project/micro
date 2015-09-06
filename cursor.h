/*
 * cursor.h
 *
 *  Created on: Aug 31, 2015
 *      Author: miguel
 */

#ifndef CURSOR_H_
#define CURSOR_H_

/******* Cursor mechanisms *******/
/* WARNING: they may be difficult to understand */

#define IGNORE -1
#define UP 0
#define DOWN 1
#define LEFT 0
#define RIGHT 1

/* Handles */
#define VMOVE(DIR) term->update(DIR, IGNORE)
#define HMOVE(DIR) term->update(IGNORE, DIR)
#define VMOVEN(DIR, N) for(int i=0;i<N;i++) term->update(DIR, IGNORE);
#define HMOVEN(DIR, N) for(int i=0;i<N;i++) term->update(IGNORE, DIR);

Point old;
#define GOTO(x,y) old = term->go_to(x, y)
#define RETCUR() term->go_to(old.x, old.y)
#define GOTOFIRST() term->cur.x = LEFT_MARGIN;
#define GOTOLAST() { node_t * row = thisrow(term); \
					 if(row && is_loc_void(term->cur) && list_size(row->value)) {\
						term->cur.x = list_index_of_node(row->value, list_find(row->value, K_NEWLINE)); \
						if(is_loc_void(term->cur)) \
							term->cur.x = LEFT_MARGIN; \
						}}
#define GOTOLAST_NONVOID() 	while(1) \
								if(!thiscol(term, thisrow(term))) \
									break;\
								else \
									HMOVE(RIGHT);

#define UNDERFLOW() uint8_t under = 0; if(IS_CURSOR_ON_START() && render_y_off + (term->cur.y - TOP_MARGIN)) { \
							node_t * prevrow = thisrow(term)->prev; \
							term->cur.y--; \
							term->cur.x = list_index_of_node(prevrow->value, list_find(prevrow->value, K_NEWLINE)) + 1 + LEFT_MARGIN; \
							under=1; \
							}

#define OVERFLOW(row) 	{if(thisrow(term)->next) { \
							term->cur.y++; \
							term->cur.x = LEFT_MARGIN; \
						}}

#define HSCROLL() if(term->cur.x < LEFT_MARGIN * 2 && render_x_off) { cursor_scroll_left(term);

#define HSCROLL_FINDLAST_ON_VIEW() for(int i=term->cur.x; i < term->size.x && !is_loc_void(term->cur);i++) \
										HMOVE(RIGHT); \
									HMOVEN(LEFT, 1);

#define VSCROLL() if(term->cur.y < TOP_MARGIN + 1 && render_y_off) { cursor_scroll_up();

#define FALL_FORWARD() int line_size = list_size(thisrow(term)->value); \
					   for(int i=0; i < (int)(line_size / term->size.x) + 1;i++) \
						   cursor_scroll_right(term); \
					    fallback = FALLFORW;

#endif /* CURSOR_H_ */
