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
#include <boost/format.hpp>
#include "Ticks.h"

// delay ms milliseconds and redturn current time afterwards
ticks_millis_t delay(int ms);
ticks_millis_t random_delay(int max);

using namespace std;
using namespace boost;

// #include <boost/test/output_test_stream.hpp>
// boost::test_tools::output_test_stream output;

#define output cout
