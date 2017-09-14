/*
 * main.h
 *
 *  Created on: 18 mei 2015
 *      Author: Elco
 */

#pragma once

#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <iostream>
#include <sstream>
#include "Ticks.h"

// delay ms milliseconds and return current time afterwards
ticks_millis_t delay(int ms);
ticks_millis_t random_delay(int max);

// output can be redirected, initialized to cout in runner.
extern std::ostream * output;
