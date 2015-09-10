/*
 * modal.c
 *
 *  Created on: Sep 7, 2015
 *      Author: miguel
 */

#include "defs.h"
#include "window.h"
#include "key.h"
#include <curses.h>
#include <string.h>
#include <stdlib.h>

int8_t modal_ret = -1;
uint8_t modal_drawn = 0;

Point editor_cursor;

Point old;
#define GOTO(x,y) old = file->term->go_to(x, y)
#define RETCUR() file->term->go_to(old.x, old.y)

/* Specific modal data and functions: */

/* Modal with textbox: */
uint8_t modal_is_input_sel = 0;
uint8_t modal_but_sel = 0;

void modal_texbox_sel_buttons() {
	modal_is_input_sel = 0;
	modal_but_sel = 1;
}

void modal_texbox_sel_input() {
	modal_is_input_sel = 1;
	modal_but_sel = 0;
}

void modal_texbox_move(uint8_t dir) {
	/*1 = Cancel/left button selected
	 *2 = Save/right button selected */
	if(dir == MOD_MOV_RIGHT && modal_but_sel == 1)
		modal_but_sel = 2;
	else if(dir == MOD_MOV_LEFT && modal_but_sel == 2)
		modal_but_sel = 1;
}

/* Global functions for modals: */

void draw_box(file_t * file) {
	int bot_marg = file->term->size.y - BOTTOM_MARGIN;
	int right_marg = file->term->size.x - RIGHT_MARGIN;

	attron(COLOR_PAIR(3));

	for(int x = LEFT_MARGIN; x < right_marg; x++) {
		GOTO(x, TOP_MARGIN);
		addch(' ');
		GOTO(x, bot_marg);
		addch(' ');
	}

	for(int y = TOP_MARGIN; y <= bot_marg; y++) {
		GOTO(LEFT_MARGIN, y);
		addch(' ');
		GOTO(right_marg,  y);
		addch(' ');
	}

	attroff(COLOR_PAIR(3));

	attron(COLOR_PAIR(1) | A_REVERSE | A_BOLD);

	for(int y = TOP_MARGIN + 1; y <= bot_marg - 1; y++) {
		for(int x = LEFT_MARGIN + 1; x <= right_marg - 1; x++) {
			GOTO(x, y);
			addch(' ');
		}
	}

	attroff(COLOR_PAIR(1) | A_REVERSE | A_BOLD);
}

void draw_header(file_t * file) {
	attron(A_REVERSE | A_BOLD);

	int r_marg = file->term->size.x - RIGHT_MARGIN;

	GOTO(LEFT_MARGIN + ((r_marg - LEFT_MARGIN)/2) - strlen(file->modal.header)/2 - 2, TOP_MARGIN);
	addstr(" ");
	addstr(file->modal.header);
	addstr(" ");
}

void draw_footer(file_t * file) {
	if(file->modal.footer) {
		GOTO(LEFT_MARGIN + 2, file->term->size.y - BOTTOM_MARGIN);
		addstr(" ");
		addstr(file->modal.footer);
		addstr(" ");
	}
}

void draw_body(file_t * file) {
	int body_left = LEFT_MARGIN + 4;
	int body_top = TOP_MARGIN + 2;
	GOTO(body_left,body_top);

	int box_width = (file->term->size.x - RIGHT_MARGIN) - body_left;
	int body_lpad = 0;
	int body_rmarg = 5;
	for(int i=0, y = 0;i<file->modal.bodysize;i++, y++) {
		if(!file->modal.body[i]) continue;

		for(int c = 0, len = 0; c < strlen(file->modal.body[i]); c++, len++) {
			GOTO(body_left + body_lpad, body_top + y);
			addch(file->modal.body[i][c]);
			if(len > box_width - body_rmarg) {
				y++;
				body_lpad = 1;
				len = 0;
			}
			body_lpad ++;
		}
		body_lpad = 0;
	}

	attroff(A_REVERSE | A_BOLD);
}

void draw_input(file_t * file) {
	int input_bot =  file->term->size.y - (BOTTOM_MARGIN + 2);
	int input_right = file->term->size.x - (RIGHT_MARGIN + 1);
	int input_left = LEFT_MARGIN + 2;

	switch(file->modal.type) {
	case MOD_TEXTBOX:
		attron(COLOR_PAIR(1));
		GOTO(input_left, input_bot - 2);
		addstr(" > ");
		for(int i=3;i < input_right - input_left; i++)
			addch(' ');

		/* Print user input: */
		GOTO(input_left + 3, input_bot - 2);
		node_t * row = (node_t*)list_get(file->modbuff, 0); /* Should be only 1 line */
		if(row) {
			int ctr = 0;
			int input_len = list_size(row->value);
			foreach(ch, (list_t*)(row->value)) {
				if(ctr++ >= input_len - 1)
					break;
				addch((char)(ch->value));
			}
		}
		attroff(COLOR_PAIR(1));


		/* Buttons: */
		if(modal_but_sel == 1)
			attron(COLOR_PAIR(5) | A_BOLD);
		else
			attron(COLOR_PAIR(3));
		GOTO(input_left + 10, input_bot);
		addstr(" Cancel ");
		attroff(COLOR_PAIR(3) | COLOR_PAIR(5) | A_BOLD);

		if(modal_but_sel == 2)
			attron(COLOR_PAIR(5) | A_BOLD);
		else
			attron(COLOR_PAIR(3));
		GOTO(input_right - 16, input_bot);
		addstr(" Save ");
		attroff(COLOR_PAIR(3) | COLOR_PAIR(5) | A_BOLD);

		/* Now position cursor: */
		if(!modal_drawn) {
			modal_is_input_sel = 1;
			GOTO(input_left + 3, input_bot - 2);
			file->term->cur.x = input_left - 2;
			file->term->cur.y = input_bot - 11;
		} else {
			GOTO(file->term->cur.x + 5, input_bot - 2);
		}
		break;
	case MOD_RUN:

		break;
	case MOD_TIMEOUT:

		break;
	case MOD_YESNO:

		break;
	case MOD_OK:

		break;
	}
}

void render_modal(file_t * file) {
	if(!modal_drawn) {
		/* Draw box: */
		draw_box(file);

		/* Header and footer: */
		draw_header(file);
		draw_footer(file);

		/* And the body */
		draw_body(file);
	} /* else: the window is already drawn. it's not necessary to draw it again UNLESS a change to the modal has been made */

	/* Now the input textbox (depends on the modal type) */
	draw_input(file);

	/* The window is now fully drawn. Shrink the window padding to fit only the input */
	modal_drawn = 1;
}

void show_modal(file_t * file, modal_t modal) {
	editor_cursor = file->term->cur;
	/* only sets the flag so that the modal will be rendered on the next superloop iteration */
	file->is_modal = 1;
	file->modal = modal;

	/* Prepare new window borders: */
	TOP_MARGIN = BOTTOM_MARGIN = file->term->size.y / 4;
	if(modal.type!=MOD_RUN)
		LEFT_MARGIN = RIGHT_MARGIN = file->term->size.x / 4;
	else
		LEFT_MARGIN = RIGHT_MARGIN = file->term->size.x / 7;
}

int8_t close_modal(file_t * file) {
	modal_drawn = 0;

	modal_is_input_sel = 0;

	set_window_border_defaults();
	GOTO(editor_cursor.x, editor_cursor.y);
	file->term->cur = editor_cursor;

	free(file->modal.header);
	for(int i=0;i<file->modal.bodysize;i++)
		free(file->modal.body[i]);
	free(file->modal.body);
	free(file->modal.footer);

	file->is_modal = 0;
	remall_callbacks();

	if(!modal_is_input_sel && file->modal.type == MOD_TEXTBOX)
		modal_ret = MODR_SAVEAS;
	if(modal_but_sel == 1)
		modal_ret = MODR_CANCEL;

	return modal_ret;
}
