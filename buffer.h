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
#include "micro_file.h"

extern list_t * init_buff();
extern void clean_buff(list_t * buff);
extern void push_buff(file_t * file, Point cursorPos, int c);
extern node_t * thisrow(file_t * file);
extern node_t * nextrow(file_t * file);
extern node_t * prevrow(file_t * file);
extern node_t * thiscol(file_t * file, node_t * row);
extern node_t * nextcol(file_t * file, node_t * row);
extern node_t * prevcol(file_t * file, node_t * row);

#endif /* BUFFER_H_ */
