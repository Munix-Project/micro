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

int render_x_off = 0;
int render_y_off = 0;

Point old;
#define GOTO(x,y) old = term->go_to(x, y)
#define RETCUR() term->go_to(old.x, old.y)

#define TOP_MARGIN_ATTR A_BOLD | A_REVERSE

void init_renderer() {
	init_buff();
}

void clean_renderer() {
	clean_buff();
}

void draw_top_margin(term_t * term) {
	attron(TOP_MARGIN_ATTR);

	Point oldp = term->cur;
	GOTO(0,0);
	for(int i=0;i<term->size.x;i++)
		addch(' ');

	GOTO(3,0); addstr(MICRO_NAME);
	char str[] = NEW_FILE;
	GOTO((term->size.x / 2) - (strlen(str) / 2),0); addstr(str);

	//attroff(TOP_MARGIN_ATTR);
	GOTO(oldp.x, oldp.y);
}

void render_all(term_t * term) {
	/* Render whole buffer from start to end
	 * (respecting the window constraints and
	 * the sizes for each line and column) */
	if(render_x_off < 0) render_x_off = 0;
	if(render_y_off < 0) render_y_off = 0;

	term->clr();

	draw_top_margin(term);

	Point old_cur = term->cur;

	int row_count = list_size(micro_buff);

	for(int y = 0, by = render_y_off; y < row_count; y++, by++) {
		node_t * row_node = list_get(micro_buff, by);
		if(!row_node) break;

		list_t * row = row_node->value;
		int col_count = list_size(row);

		for(int x = 0, bx = render_x_off; x <= term->size.x - RIGHT_MARGIN; x++, bx++) {
			node_t * node_char = list_get(row, bx);
			if(!node_char) break;

			int char_ = node_char->value;
			GOTO(x, y + TOP_MARGIN);
			addch(char_);
		}
	}

	GOTO(old_cur.x, old_cur.y);

	fflush(stdout);
}

uint8_t is_loc_void(Point loc) {
	node_t * rownode = list_get(micro_buff, render_y_off + loc.y - TOP_MARGIN);
	if(rownode) {
		loc.x += render_x_off;
		node_t * row_loc = list_get(rownode->value, loc.x - 1 < 0 ? loc.x : loc.x - 1);
		if(row_loc && row_loc->value!=K_NEWLINE && row_loc->value != K_CARRIAGE)
			return 0;
		else
			return 1;
	} else {
		return 1;
	}
}

void update_cursor_visual(term_t * term, Point old_cursor) {
	if(!is_loc_void(term->cur)) {
		GOTO(term->cur.x, term->cur.y);
	} else {
		/* Allow cursor to jump to next line. This is because the newline has just '\r' in it, and \r is considered a void cell */
		node_t * thisr = thisrow(term);
		node_t * thiscolumn = NULL;
		if(thisr)
			thiscolumn = thiscol(term, thisr);
		if(thiscolumn) {
			if(thiscolumn->value != K_CARRIAGE && thiscolumn->value != K_NEWLINE)
				term->cur = old_cursor;
		} else {
			term->cur = old_cursor;
		}
	}
}
