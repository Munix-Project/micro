/*
 * render.c
 *
 *  Created on: Aug 31, 2015
 *      Author: miguel
 */

#include "render.h"
#include "buffer.h"
#include <ncurses.h>
#include <stdio.h>
#include <signal.h>

Point old;
#define GOTO(x,y) old = term->go_to(x, y)
#define RETCUR() term->go_to(old.x, old.y)

void init_renderer() {
	init_buff();
}

void clean_renderer() {
	clean_buff();
}

void render_all(term_t * term) {
	/* Render whole buffer from start to end
	 * (respecting the window constraints and
	 * the sizes for each line and column) */

	Point old_cur = term->cur;
	int x = 0, y = 0;
	foreach(row, micro_buff.buff) {
		foreach(col, (list_t*)row->value) {
			int char_ = col->value;
			GOTO(x, y);
			addch(char_);
			if(char_ == '\n') break;
			x++;
		}
		x = 0;
		y++;
	}

	GOTO(old_cur.x, old_cur.y);

	fflush(stdout);
}

uint8_t is_loc_void(Point loc) {
	node_t * rownode = list_get(micro_buff.buff, loc.y);
	if(rownode) {
		node_t * row_loc = list_get(rownode->value, loc.x-1 < 0 ? loc.x : loc.x - 1);
		if(row_loc)
			return 0;
		else
			return 1;
	} else {
		return 1;
	}
}

void update_cursor_visual(term_t * term, Point old_cursor) {
	if(!is_loc_void(term->cur))
		GOTO(term->cur.x, term->cur.y);
	else
		term->cur = old_cursor;
}
