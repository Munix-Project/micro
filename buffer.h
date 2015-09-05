/*
 * buffer.h
 *
 *  Created on: Aug 31, 2015
 *      Author: miguel
 */

#ifndef BUFFER_H_
#define BUFFER_H_

#include "defs.h"
#include "list.h"
#include "term.h"

extern list_t * micro_buff;

extern void init_buff();
extern void clean_buff();
extern void push_buff(Point cursorPos, int c);
extern node_t * thisrow(term_t * term);
extern node_t * nextrow(term_t * term);
extern node_t * prevrow(term_t * term);
extern node_t * thiscol(term_t * term, node_t * row);
extern node_t * nextcol(term_t * term, node_t * row);
extern node_t * prevcol(term_t * term, node_t * row);

#endif /* BUFFER_H_ */
