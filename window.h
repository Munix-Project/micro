/*
 * window.h
 *
 *  Created on: Sep 1, 2015
 *      Author: miguel
 */

#ifndef WINDOW_H_
#define WINDOW_H_

#define TOP_MARGIN 2
#define LEFT_MARGIN 5
#define RIGHT_MARGIN 3
#define BOTTOM_MARGIN 3
#define DELTA_RIGHT_SCROLL (file->term->size.x - (LEFT_MARGIN * 2))
#define DELTA_LEFT_SCROLL DELTA_RIGHT_SCROLL
#define DELTA_TOP_SCROLL 1
#define DELTA_BOTTOM_SCROLL 1

typedef struct {
	int x_off;
	int y_off;
} render_t;

#endif /* WINDOW_H_ */
