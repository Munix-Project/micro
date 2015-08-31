/*
 * key.h
 *
 *  Created on: Aug 30, 2015
 *      Author: miguel
 */

#ifndef KEY_H_
#define KEY_H_

#include "term.h"

/* keys that don't belong to ncurses */
enum KEYS{
	K_NEWLINE 	= 	10,
	K_BACKSPACE =	127,
	K_ESC 		= 	27,
	K_UP 		= 	'A',
	K_DOWN 		= 	'B',
	K_RIGHT 	= 	'C',
	K_LEFT 		= 	'D'
};

extern void key_handle(term_t * term);

#endif /* KEY_H_ */
