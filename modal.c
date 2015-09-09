/*
 * modal.c
 *
 *  Created on: Sep 7, 2015
 *      Author: miguel
 */

#include "defs.h"
#include "window.h"
#include <curses.h>
#include <string.h>

Point old;
#define GOTO(x,y) old = file->term->go_to(x, y)
#define RETCUR() file->term->go_to(old.x, old.y)

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

		attroff(COLOR_PAIR(1));
		attron(COLOR_PAIR(3));
		GOTO(input_left + 10, input_bot);
		addstr(" Cancel ");

		GOTO(input_right - 16, input_bot);
		addstr(" Save ");
		attroff(COLOR_PAIR(3));

		/* Now position cursor: */
		GOTO(input_left + 3, input_bot - 2);
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
	/* Draw box: */
	draw_box(file);

	/* Header and footer: */
	draw_header(file);
	draw_footer(file);

	/* And the body */
	draw_body(file);

	/* Now the input textbox (depends on the modal type) */
	draw_input(file);
}

void show_modal(file_t * file, modal_t modal) {
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

void close_modal(file_t * file) {
	set_window_border_defaults();

	free(file->modal.header);
	for(int i=0;i<file->modal.bodysize;i++)
		free(file->modal.body[i]);
	free(file->modal.body);
	free(file->modal.footer);

	file->is_modal = 0;
}
