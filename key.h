/*
 * key.h
 *
 *  Created on: Aug 30, 2015
 *      Author: miguel
 */

#ifndef KEY_H_
#define KEY_H_

#include "defs.h"
#include "cursor.h"

/* keys that don't belong to ncurses */
enum KEYS {
	K_NEWLINE 	= 	10,
	K_BACKSPACE =	127,
	K_DEL		=	126,
	K_ESC 		= 	27,
	K_UP 		= 	'A',
	K_DOWN 		= 	'B',
	K_RIGHT 	= 	'C',
	K_LEFT 		= 	'D'
};

enum KEY_TYPE {
	KT_NORMAL,
	KT_ESC,
	KT_MOD
};

enum KEY_CALLBACK_RET {
	KR_SUCCESS,
	KR_ERR,
	KR_CONT
};

extern void init_keys();
extern void clean_keys();
extern void key_handle(file_t * file);

typedef int (*key_cback)(int, uint8_t);

extern void add_callback(int id, key_cback func_ptr);
extern key_cback get_callback(int id);
extern void rem_callback(int id);
extern void remall_callbacks();

/* These functions might be useful for the 'function' c file.
 * Because they abstract the mechanisms of the terminal */
extern void handle_normal(file_t * file, int c);
extern void handle_escape(file_t * file, int escode);
extern void handle_modifier(int c);

#endif /* KEY_H_ */
