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

typedef struct {
	list_t * buff;
	list_t * sizes;
} mbuff_t;

extern mbuff_t micro_buff;

extern void init_buff();
extern void clean_buff();
extern void push_buff(Point cursorPos, int c);

#endif /* BUFFER_H_ */