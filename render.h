/*
 * render.h
 *
 *  Created on: Aug 31, 2015
 *      Author: miguel
 */

#ifndef RENDER_H_
#define RENDER_H_

#include "defs.h"
#include "window.h"

extern render_t * init_renderer();
extern void clean_renderer(render_t * renderer);
extern void render_all(file_t * file);
extern uint8_t is_loc_void(file_t* file, Point loc);
extern void update_cursor_visual(file_t * file, Point old_cursor);

#endif /* RENDER_H_ */
