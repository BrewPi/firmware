/* 
 * File:   ScrollBox.cpp
 * Author: Elco
 * 
 * Created on 4 november 2014, 14:19
 */

#include "ScrollBox.h"
#include "../Adafruit_ILI9341/Adafruit_ILI9341.h"

ScrollBox::ScrollBox(Adafruit_ILI9341 * mytft) : tft(mytft) {
    xpos = 0;
    ypos = 200;
    width = 320;
    height = 40;
    lines = 5;
    lineWidth = 50;
    currIndex = 0;
            
    // Allocate buffer
    if (( buf = ( char** )malloc( lines *sizeof( char *))) == NULL )
        { /* error */ }
    
    for (uint16_t i = 0; i < lines; i++ ){
        if (( buf[i] = ( char* )malloc( lineWidth * sizeof( char ) )) == NULL )
            { /* error */ }
        buf[i][0] = '\0';
    }
}

ScrollBox::~ScrollBox() {
    for (uint8_t i = 0; i < lines; i++ ){
        free(buf[i]);
    }
    free(buf);
}

void ScrollBox::scroll(void) {  
    uint8_t i;
    char * emptiedLine = buf[0];
    emptiedLine[0]='\0';
    for (i = 0; i < lines - 1; i++ ){
        buf[i] = buf[i+1];
    }
    // rotate line buffers
    buf[lines - 1] = emptiedLine; // last line gets emptied buffer
    currIndex = 0;
}

size_t ScrollBox::write(uint8_t c) {
    if (c == '\n') {
        // update display
        update();
        // scroll up all text in buffer
        scroll();
    } else if (c == '\r') {
        // skip em
    } else {
        // copy character to buffer
        buf[lines-1][currIndex] = c;
        buf[lines-1][currIndex+1] = '\0';
        currIndex++;
    }
    if (currIndex > lineWidth){
        // continue on next line
        scroll();
    }
    return 1;
}

void ScrollBox::printSpaces(uint16_t num) {
    for(uint16_t i = 0; i < num;i++){
        tft->write(' ');
    }
}

void ScrollBox::update(void) {
    // updates the display with the content of the buffer
    tft->setCursor(xpos, ypos);
    tft->setTextColor(ILI9341_WHITE, ILI9341_BLACK);
    for(uint16_t i = 0; i < lines;i++){
        for(uint16_t j = 0; j < lineWidth;j++){
            uint8_t c = buf[i][j];
            if( c == '\0' ){
                // fill rest of line with spaces to erase previous text
                printSpaces(lineWidth-j);
                break;
            }
            else{
                tft->write(c);
            }            
        }
        tft->write('\n');
    }
}
