/*
 * render.c
 *
 *  Created on: Aug 31, 2015
 *      Author: miguel
 */

#include "render.h"
#include "buffer.h"
#include "key.h"
#include "window.h"
#include "defs.h"
#include <stdio.h>
#include <string.h>
#include <curses.h>
#include <stdlib.h>

int TOP_MARGIN 		= DEFAULT_TMARG;
int LEFT_MARGIN 	= DEFAULT_LMARG;
int RIGHT_MARGIN 	= DEFAULT_RMARG;
int BOTTOM_MARGIN 	= DEFAULT_BMARG;

Point old;
#define GOTO(x,y) old = file->term->go_to(x, y)
#define RETCUR() file->term->go_to(old.x, old.y)

#define TOP_MARGIN_ATTR A_BOLD | A_REVERSE
#define BOT_MARGIN_ATTR A_REVERSE
#define LEF_MARGIN_ATTR A_REVERSE

extern void render_modal(file_t * file);

void set_window_border_defaults() {
	TOP_MARGIN 		= DEFAULT_TMARG;
	LEFT_MARGIN 	= DEFAULT_LMARG;
	RIGHT_MARGIN 	= DEFAULT_RMARG;
	BOTTOM_MARGIN 	= DEFAULT_BMARG;
}

render_t * init_renderer() {
	render_t * new_renderer = malloc(sizeof(render_t));
	new_renderer->x_off = 0;
	new_renderer->y_off = 0;

	start_color();
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	init_pair(2, COLOR_WHITE, COLOR_BLACK);
	init_pair(3, COLOR_BLACK, COLOR_WHITE);
	init_pair(4, COLOR_BLACK, COLOR_WHITE);
	init_pair(5, COLOR_WHITE, COLOR_CYAN);

	attron(COLOR_PAIR(2));

	return new_renderer;
}

void clean_renderer(render_t * renderer) {
	free(renderer);
}

void draw_top_margin(file_t * file) {
	attron(TOP_MARGIN_ATTR);

	Point oldp = file->term->cur;
	GOTO(0,0);
	for(int i=0;i<file->term->size.x;i++)
		addch(' ');

	GOTO(3,0); addstr(MICRO_NAME);
	char str[] = NEW_FILE;
	GOTO((file->term->size.x / 2) - (strlen(str) / 2),0); addstr(str);

	attroff(TOP_MARGIN_ATTR);
	GOTO(oldp.x, oldp.y);
}

void draw_left_margin(file_t * file) {
	Point oldp = file->term->cur;

	for(int y=TOP_MARGIN; y < file->term->size.y - BOTTOM_MARGIN && (y - TOP_MARGIN) + file->rend->y_off < list_size(file->is_modal ? file->modbuff : file->buff); y++) {

		char itos[LEFT_MARGIN];
		sprintf(itos, "%d", (y - TOP_MARGIN + 1) + file->rend->y_off);
		int left_space = strlen(itos);
		int x_pos = LEFT_MARGIN - left_space - 1;

		if(y == file->term->cur.y){
			GOTO(0, y);
			attron(COLOR_PAIR(1));
			attron(LEF_MARGIN_ATTR);
			for(int i=0;i<LEFT_MARGIN-1;i++)
				addch(' ');
			GOTO(x_pos, y);
			addstr(itos);
			attroff(COLOR_PAIR(1));
			attroff(LEF_MARGIN_ATTR);
		} else {
			GOTO(x_pos, y);
			addstr(itos);
		}
	}

	GOTO(oldp.x, oldp.y);
}

void botmargin_showopt(char opt, char * detail, uint8_t left_marg, int8_t right_pad) {
	if(left_marg)
		for(int i=0;i<left_marg;i++)
		addch(' ');
	attron(A_BOLD);
	attron(BOT_MARGIN_ATTR);
	addch(opt);
	attroff(A_BOLD);
	addch(' ');
	addstr(detail);
	if(right_pad != -1) {
		addch(' ');
		if(right_pad > 0)
			for(int i=0;i<right_pad;i++)
				addch(' ');
	}
	attroff(BOT_MARGIN_ATTR);
	addch(' ');
}

void draw_bottom_margin(file_t * file) {
	Point oldp = file->term->cur;

	int y_margin = file->term->size.y - BOTTOM_MARGIN;

	GOTO(0, y_margin + 1);
	addstr("CTRL+");
	botmargin_showopt(F_SAVE, 	  "Save", 	 	  0, 0);
	botmargin_showopt(F_SAVEAS,   "Save As", 	  0, 0);
	botmargin_showopt(F_UNDO, 	  "Undo", 	 	  0, 1);
	botmargin_showopt(F_EXIT,	  "Exit", 	 	  0, 1);
	botmargin_showopt(F_FIND, 	  "Find", 	 	  0, 0);
	botmargin_showopt(F_COPY, 	  "Copy", 	 	  0, 0);
	botmargin_showopt(F_CUT, 	  "Cut", 	 	  0, 0);
	botmargin_showopt(F_PASTE,	  "Paste", 	 	  0, 0);
	botmargin_showopt(F_GOTO, 	  "Goto", 	 	  0, -1);
	botmargin_showopt(F_RUN, 	  "Run", 	 	  4, 1);
	botmargin_showopt(F_HELP, 	  "Help", 	 	  0, 3);
	botmargin_showopt(F_NEXTPAGE, "NPage", 	 	  0, 0);
	botmargin_showopt(F_PREVPAGE, "PPage", 	 	  0, 0);
	botmargin_showopt(F_OPEN, 	  "Open", 	 	  0, 0);
	botmargin_showopt(F_NEW, 	  "New", 		  0, 1);
	botmargin_showopt(F_MORE, 	  "    More    ", 0, 0);

	/* Add these to the 'next' more: */
	/*botmargin_showopt("K", "NFile", 0, 1);
	botmargin_showopt("J", "PFile", 0, 1);*/

	/* Draw x cursor position: */
	char cur_str[5];
	sprintf(cur_str, "C: %d", file->rend->x_off + (file->term->cur.x - LEFT_MARGIN) + 1);
	addstr(cur_str);

	GOTO(oldp.x, oldp.y);
}

void render_editor(file_t * file) {
	draw_top_margin(file);
	draw_left_margin(file);
	draw_bottom_margin(file);

	Point old_cur = file->term->cur;

	for(int y = TOP_MARGIN, by = file->rend->y_off; y < file->term->size.y - BOTTOM_MARGIN; y++, by++) {
		node_t * row_node = list_get(file->buff, by);
		if(!row_node) break;

		list_t * row = row_node->value;

		int x = LEFT_MARGIN;
		int x_max =  file->term->size.x - RIGHT_MARGIN;
		for(int bx = file->rend->x_off; x <= x_max; x++, bx++) {
			node_t * node_char = list_get(row, bx);
			if(!node_char) break;

			int char_ = node_char->value;
			GOTO(x, y);
			addch(char_);

			/* Draw arrows indicating the continuation of text outside of render view */
			/* Right border: */
			if(x == x_max && node_char->next && node_char->next->next) {
				attron(A_REVERSE);
				addch('>');
				attroff(A_REVERSE);
			}

			/* Left border: */
			if(file->rend->x_off) {
				move(y, LEFT_MARGIN-1);
				attron(A_BOLD | A_REVERSE);
				addch('<');
				attroff(A_BOLD | A_REVERSE);
				move(x,y);
			}
		}
	}

	GOTO(old_cur.x, old_cur.y);
}

void render_all(file_t * file) {
	/* Render whole buffer from start to end
	 * (respecting the window constraints and
	 * the sizes for each line and column) */

	/* Switch between contexts. We might render the editor OR a menu which may belong to a function */
	if(!file->is_modal) {
		file->term->clr();
		render_editor(file);
	} else {
		/* else we're in a modal. the file modal.c handles that. */
		render_modal(file);
	}

	fflush(stdout);
}

uint8_t is_loc_void(file_t * file, Point loc) {
	node_t * rownode = list_get( file->is_modal ? file->modbuff : file->buff,
								(file->is_modal ? 0 : file->rend->y_off) + (loc.y - TOP_MARGIN));

	if(rownode) {
		loc.x = (file->is_modal ? 0 : file->rend->x_off) + (loc.x - LEFT_MARGIN);

		node_t * row_loc = list_get(rownode->value, loc.x - 1 < 0 ? loc.x : loc.x - 1);
		if(row_loc && row_loc->value != K_NEWLINE)
			return 0;
		else
			return 1;
	} else {
		return 1;
	}
}

void update_cursor_visual(file_t * file, Point old_cursor) {
	if(!is_loc_void(file, file->term->cur)) {
		GOTO(file->term->cur.x, file->term->cur.y);
	} else {
		/* Allow cursor to jump to next line. This is because the newline has just '\r' in it, and \r is considered a void cell */
		node_t * thisr = thisrow(file);
		node_t * thiscolumn = NULL;
		if(thisr)
			thiscolumn = thiscol(file, thisr);
		if(thiscolumn) {
			if(thiscolumn->value != K_NEWLINE)
				file->term->cur = old_cursor;
		} else {
			file->term->cur = old_cursor;
		}
	}
}
