/*
 * window.h
 *
 *  Created on: Sep 1, 2015
 *      Author: miguel
 */

#ifndef WINDOW_H_
#define WINDOW_H_

#include <stdint.h>

#define DEFAULT_TMARG 2
#define DEFAULT_LMARG 5
#define DEFAULT_RMARG 3
#define DEFAULT_BMARG 3

extern int TOP_MARGIN;
extern int LEFT_MARGIN;
extern int RIGHT_MARGIN;
extern int BOTTOM_MARGIN;

#define DELTA_RIGHT_SCROLL (file->term->size.x - (LEFT_MARGIN * 2))
#define DELTA_LEFT_SCROLL DELTA_RIGHT_SCROLL
#define DELTA_TOP_SCROLL 1
#define DELTA_BOTTOM_SCROLL 1

#define MODAL_MAXBUFF 34

typedef struct {
	int x_off;
	int y_off;
} render_t;

extern void set_window_border_defaults(void);

/* Modal data */

enum MODAL_TYPE {
	MOD_OK,
	MOD_YESNO,
	MOD_TEXTBOX,
	MOD_TIMEOUT,
	MOD_RUN
};

typedef struct {
	char * header;
	char ** body;
	char * footer;
	uint16_t bodysize;
	uint8_t type;
} modal_t;

enum MODAL_MOV{
	MOD_MOV_RIGHT,
	MOD_MOV_LEFT
};

enum MODAL_RET {
	MODR_CANCEL,
	MODR_SAVEAS,
	MODR_OK,
	MODR_YES,
	MODR_NO
};

extern uint8_t modal_is_input_sel;
extern void modal_texbox_sel_buttons();
extern void modal_texbox_sel_input();
extern void modal_texbox_move(uint8_t dir);

#endif /* WINDOW_H_ */
