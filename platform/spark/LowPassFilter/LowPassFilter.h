/*
 * Copyright 2014 BrewPi/Elco Jacobs.
 *
 * This file is part of BrewPi.
 * 
 * BrewPi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * BrewPi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with BrewPi.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LOWPASSFILTER_H
#define LOWPASSFILTER_H

/* This class implements an IIR low pass filter, with the following frequency response

                                1  +		  2 z^-1  +					1 z^-2
H(z) = 2^-a * -------------------------------------------------
                                1  + (-2 + 2^-b)z^-1  + (1-2^-b + 4* 2^-a)z^-2

 All filter coefficients are powers of two, so the filter can be efficiently implemented with bit shifts
 The DC gain is exactly 1.
 For real poles, and therefore no overshoot, use a <= 2b+4.
 To calculate the poles, you can use this wolfram alpha link:
 http://www.wolframalpha.com/input/?i=solve+%281++%2B+%28-2+%2B+2^-b%29z^-1++%2B+%281-2^-b+%2B+4*+2^-a%29z^-2%29+%3D+0+where+a+%3D+24+and+b+%3D+10
 The filter has a zero at z = -1
 For a=2b+4, it has a pole at z = (2^(b+1)-1) / 2^(b+1)
 Use this MATLAB script to visualize the filter:
         a=12; b=4; FS=1;
         DEN = [1  , -2 + 2^-b  , 1-2^-b+ 4*2^-a]; NUM = 2^(-a)*[1, 2, 1];
         h=fvtool(NUM,DEN,'Fs',FS)
         zeropos = roots(NUM)
         polepos = roots(DEN)
         set(h,'FrequencyRange', 'Specify freq. vector');
         set(h,'FrequencyScale','Log')
         set(h,'FrequencyVector', logspace(-4,0,1000));
 */

#include <inttypes.h>
// defines for a filter optimal step response without overshoot: a,b
#define SETTLING_TIME_25_SAMPLES	0x0601
#define SETTLING_TIME_50_SAMPLES	0x0802
#define SETTLING_TIME_100_SAMPLES	0x0A03
#define SETTLING_TIME_200_SAMPLES	0x0C04
#define SETTLING_TIME_400_SAMPLES	0x0E05
#define SETTLING_TIME_800_SAMPLES	0x1006

class LowPassFilter {
public:
    // input and output arrays
    int32_t xv[3];
    int32_t yv[3];
    uint8_t a;
    uint8_t b;

public:
    LowPassFilter();
    ~LowPassFilter();
    void init(int16_t val);
    void setCoefficients(uint8_t a, uint8_t b);
    void setCoefficients(uint16_t ab);
    int16_t add(int16_t val); // adds a value and returns the most recent filter output
    int32_t addDoublePrecision(int32_t val);
    int16_t readInput(void); // returns the most recent filter input
    int16_t readOutput(void); // returns the most recent filter output
    int32_t readOutputDoublePrecision(void);
    int32_t readPrevOutputDoublePrecision(void);
};

#endif /* LOWPASSFILTER_H */