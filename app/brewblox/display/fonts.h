/*
 * Copyright 2015 BrewPi / Elco Jacobs, Matthew McGowan.
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

#ifndef FONTS_H
#define FONTS_H

#define FONT_ARIAL7    0
#define FONT_ARIAL7_HIGH     1 
#define FONT_ARIAL7_WIDE     2
#define FONT_ARIAL7_BIG      3
//#define FONT_BERLIN_SANS_FBDEMI12         4
//#define FONT_BERLIN_SANS_FBDEMI12_HIGH    5
//#define FONT_BERLIN_SANS_FBDEMI12_BIG     6
//#define FONT_SEGOEUI8   7
//#define FONT_SEGOEUI8_BIG   8
//#define FONT_SEGOEUI9   9
//#define FONT_SEGOEUI10  10
#define FONT_SEGOEUI11  11
//#define FONT_SEGOEUI9_BIG  12
//#define FONT_PROGGY_SQUARE  13
//#define FONT_PROGGY_SQUARE_BIG  14
#define FONT_ALTE_DIN_22  15
#define FONT_ALTE_DIN_16  16

#define FONT_SMALL FONT_ARIAL7
#define FONT_REGULAR FONT_SEGOEUI11
#define FONT_NUMBER_LARGE FONT_ALTE_DIN_22
#define FONT_NUMBER_MEDIUM FONT_ALTE_DIN_16

//extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNew8B_desc;
//extern const D4D_FONT_DESCRIPTOR d4dfnt_TimesNewRoman12B_desc;
//extern const D4D_FONT_DESCRIPTOR d4dfnt_BerlinSansFBDemi12_desc;
extern const D4D_FONT_DESCRIPTOR d4dfnt_Arial7_desc;
//extern const D4D_FONT_DESCRIPTOR d4dfnt_CourierNewTest_desc;
//extern const D4D_FONT_DESCRIPTOR d4dfnt_Tahoma_8_desc;
//extern const D4D_FONT_DESCRIPTOR d4dfnt_Tahoma_9_desc;
//extern const D4D_FONT_DESCRIPTOR d4dfnt_SegoeUISemibold_9_desc;
//extern const D4D_FONT_DESCRIPTOR d4dfnt_SegoeUI_9_desc;
//extern const D4D_FONT_DESCRIPTOR d4dfnt_SegoeUI_8_desc;
//extern const D4D_FONT_DESCRIPTOR d4dfnt_Verdana_8_desc;
extern const D4D_FONT_DESCRIPTOR d4dfnt_SegoeUI11_desc;
//extern const D4D_FONT_DESCRIPTOR d4dfnt_SegoeUI10_desc;
//extern const D4D_FONT_DESCRIPTOR d4dfnt_ProggySquare_desc;
extern const D4D_FONT_DESCRIPTOR d4dfnt_AlteDIN1451_22_desc;
extern const D4D_FONT_DESCRIPTOR d4dfnt_AlteDIN1451_16_desc;


#endif