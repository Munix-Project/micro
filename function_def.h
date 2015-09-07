/*
 * function_def.h
 *
 *  Created on: Sep 6, 2015
 *      Author: miguel
 */

#ifndef FUNCTION_DEF_H_
#define FUNCTION_DEF_H_

#include <stdint.h>

enum FUNC_T {
	F_SAVE 		= 'S',
	F_SAVEAS 	= 'X',
	F_UNDO 		= 'Z',
	F_EXIT 		= 'D',
	F_FIND 		= 'F',
	F_COPY 		= 'C',
	F_CUT 		= 'B',
	F_PASTE 	= 'V',
	F_GOTO 		= 'G',
	F_RUN 		= 'R',
	F_HELP 		= 'H',
	F_NEXTPAGE 	= 'L',
	F_PREVPAGE 	= 'P',
	F_OPEN 		= 'O',
	F_NEW 		= 'N',
	F_MORE 		= 'E'
};

typedef struct {
	uint8_t quit_fl;
} status_t;

extern status_t term_status;

#endif /* FUNCTION_DEF_H_ */
