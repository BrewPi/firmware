/* 
 * File:   ScrollBox.h
 * Author: Elco Jacobs
 *
 * This class implements a scrolling debug box on a TFT display
 * With each printed line, the display will scroll the old line up
 */

#ifndef SCROLLBOX_H
#define	SCROLLBOX_H

#include "../Adafruit_ILI9341/Adafruit_ILI9341.h"
#include <stdarg.h>
#include <stdio.h>


class ScrollBox : public Print {
public:
    ScrollBox(Adafruit_ILI9341 * tft);
    virtual ~ScrollBox();
    void scroll(void);
    void update(void);
    void printSpaces(uint16_t num);
       
    virtual size_t write(uint8_t);
private:
    
    Adafruit_ILI9341 * tft;
    uint16_t xpos; // x-position of the box on the display
    uint16_t ypos; // y-position of the box on the display
    
    uint16_t width; // width of the box
    uint16_t height; // height of the box
    
    uint16_t lineWidth;
    uint16_t lines;
    
    char ** buf;
    uint16_t currIndex;
};

#endif	/* SCROLLBOX_H */
