/*
 * term.h
 *
 *  Created on: Aug 30, 2015
 *      Author: miguel
 */

#ifndef TERM_H_
#define TERM_H_

#include "global.h"
#include <stdint.h>

#define TERM_NO_AVAIL -1

typedef int (*getkey)(void);
typedef void (*clrscr)(void);
typedef Point (*cur_goto)(int, int);
typedef void (*updatecur)(uint8_t,uint8_t);

enum TERM_MODES {
	MODE_INSERT
};

#define MODE_COUNT MODE_INSERT + 1

typedef struct {
	Point cur;
	Point size;
	getkey read;
	clrscr clr;
	cur_goto go_to;
	updatecur update;
	int modes[MODE_COUNT];
} term_t;

extern term_t * init_term();
extern void clean_term(term_t * t);

#endif /* TERM_H_ */
