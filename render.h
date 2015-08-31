/*
 * render.h
 *
 *  Created on: Aug 31, 2015
 *      Author: miguel
 */

#ifndef RENDER_H_
#define RENDER_H_

#include "term.h"

extern void init_renderer();
extern void clean_renderer();
extern void render_all(term_t * term);
extern uint8_t is_loc_void(Point loc);
extern void update_cursor_visual(term_t * term, Point old_cursor);

#endif /* RENDER_H_ */
