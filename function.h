/*
 * function.h
 *
 *  Created on: Sep 6, 2015
 *      Author: miguel
 */

#ifndef FUNCTION_H_
#define FUNCTION_H_

#include "micro_file.h"
#include "function_def.h"
#include <stdint.h>

extern void init_func(file_t * t);

extern int execute_modifier(int mod);
extern void mod_quit();

#endif /* FUNCTION_H_ */
