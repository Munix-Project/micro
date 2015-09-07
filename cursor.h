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
#define VMOVE(DIR) file->term->update(DIR, IGNORE)
#define HMOVE(DIR) file->term->update(IGNORE, DIR)
#define VMOVEN(DIR, N) for(int i=0;i<N;i++) file->term->update(DIR, IGNORE);
#define HMOVEN(DIR, N) for(int i=0;i<N;i++) file->term->update(IGNORE, DIR);

Point old;
#define GOTO(x,y) old = file->term->go_to(x, y)
#define RETCUR() file->term->go_to(old.x, old.y)
#define GOTOFIRST() file->term->cur.x = LEFT_MARGIN;
#define GOTOLAST() { node_t * row = thisrow(file); \
					 if(row && is_loc_void(file, file->term->cur) && list_size(row->value)) {\
						file->term->cur.x = list_index_of_node(row->value, list_find(row->value, K_NEWLINE)); \
						if(is_loc_void(file, file->term->cur)) \
							file->term->cur.x = LEFT_MARGIN; \
						}}
#define GOTOLAST_NONVOID() 	while(1) \
								if(!thiscol(file, thisrow(file))) \
									break;\
								else \
									HMOVE(RIGHT);

#define UNDERFLOW() uint8_t under = 0; if(IS_CURSOR_ON_START() && file->rend->y_off + (file->term->cur.y - TOP_MARGIN)) { \
							node_t * prevrow = thisrow(file)->prev; \
							file->term->cur.y--; \
							file->term->cur.x = list_index_of_node(prevrow->value, list_find(prevrow->value, K_NEWLINE)) + 1 + LEFT_MARGIN; \
							under=1; \
							}

#define OVERFLOW(row) 	{if(thisrow(file)->next) { \
							file->term->cur.y++; \
							file->term->cur.x = LEFT_MARGIN; \
						}}

#define HSCROLL() if(file->term->cur.x < LEFT_MARGIN * 2 && file->rend->x_off) { cursor_scroll_left(file);

#define HSCROLL_FINDLAST_ON_VIEW() for(int i=file->term->cur.x; i < file->term->size.x && !is_loc_void(file, file->term->cur);i++) \
										HMOVE(RIGHT); \
									HMOVEN(LEFT, 1);

#define VSCROLL() if(file->term->cur.y < TOP_MARGIN + 1 && file->rend->y_off) { cursor_scroll_up(file);

#define FALL_FORWARD() int line_size = list_size(thisrow(file)->value); \
					   for(int i=0; i < (int)(line_size / file->term->size.x) + 1;i++) \
						   cursor_scroll_right(file); \
					    fallback = FALLFORW;

#endif /* CURSOR_H_ */
