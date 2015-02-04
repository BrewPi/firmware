/*
Multifont GFX library is adapted from Adafruit_GFX library by Paul Kourany
v1.0.0, May 2014 Initial Release
v1.0.1, June 2014 Font Compilation update

Please read README.pdf for details
*/

//  fonts.h

#ifndef _fonts_h
#define _fonts_h

#include "application.h"

//Font selection for compiling - comment out or uncomment definitions as required
//NOTE: GLCDFONT is default font and always included
//#define TIMESNEWROMAN8
//#define CENTURYGOTHIC8
//#define ARIAL8
//#define COMICSANSMS8
//#define TESTFONT

// Font selection descriptors - Add an entry for each new font and number sequentially
#define TIMESNR_8	0
#define CENTURY_8	1
#define ARIAL_8		2
#define COMICS_8	3
#define GLCDFONT	4
#define TEST		5

#define FONT_START 0
#define FONT_END 1

struct FontDescriptor
{
	uint8_t	width;		// width in bits
	uint8_t	height; 	// char height in bits
	uint16_t offset;	// offset of char into char array
};

// Font references - add pair of references for each new font
#ifdef TIMESNEWROMAN8
extern const uint8_t timesNewRoman_8ptBitmaps[];
extern const FontDescriptor timesNewRoman_8ptDescriptors[];
#endif

#ifdef CENTURYGOTHIC8
extern const uint8_t centuryGothic_8ptBitmaps[];
extern const FontDescriptor centuryGothic_8ptDescriptors[];
#endif

#ifdef ARIAL8
extern const uint8_t arial_8ptBitmaps[];
extern const FontDescriptor arial_8ptDescriptors[];
#endif

#ifdef COMICSANSMS8
extern const uint8_t comicSansMS_8ptBitmaps[];
extern const FontDescriptor comicSansMS_8ptDescriptors[];
#endif

extern const uint8_t glcdfontBitmaps[];
extern const FontDescriptor glcdfontDescriptors[];

#ifdef TESTFONT
extern const uint8_t testBitmaps[];
extern const FontDescriptor testDescriptors[];
#endif


#endif
