/* 
 * File:   ValvesController.h
 * Author: Elco
 *
 * Created on 16 december 2014, 15:28
 */

#pragma once
#include "../DS2408/DS2408.h"

class ValvesController : public DS2408 {
public:
    ValvesController();
    virtual ~ValvesController();
    enum{
        OFF_LOW = 0b00,
        CLOSE = 0b10,
        OPEN = 0b01,
        OFF = 0b11
    } valveActions;
    
    void update();
    uint8_t read(uint8_t valve, bool doUpdate = true);
    void write(uint8_t valve, uint8_t state);
    void open(uint8_t valve);
    void close(uint8_t valve);
    void stop(uint8_t valve);

protected:
    uint8_t senseA;
    uint8_t senseB;
    uint8_t actA;
    uint8_t actB;
};

