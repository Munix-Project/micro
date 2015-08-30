/*
 * term.h
 *
 *  Created on: Aug 30, 2015
 *      Author: miguel
 */

#ifndef TERM_H_
#define TERM_H_

#include <stdint.h>

#define TERM_NO_AVAIL -1

typedef int (*getkey)(void);
typedef void (*clrscr)(void);
typedef int (*kbavail)(void);

typedef struct {
	int x;
	int y;
} Point;

typedef struct {
	Point cur;
	Point size;
	getkey read;
	clrscr clear;
} term_t;

extern term_t init_term();
extern void clean_term(term_t t);

#endif /* TERM_H_ */
