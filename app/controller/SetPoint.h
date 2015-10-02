/*
 * Copyright 2015 BrewPi/Elco Jacobs.
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

#pragma once

#include "temperatureFormats.h"

class SetPoint {
public:
    SetPoint(){};
    ~SetPoint(){};
    virtual temp_t read() const = 0;
    virtual void write(temp_t val) = 0;
};


class SetPointSimple : public SetPoint {
public:
    SetPointSimple(temp_t val = temp_t::disabled()) : sp(val){}
    ~SetPointSimple(){};
    virtual temp_t read() const{
        return sp;
    }
    virtual void write(temp_t val){
        sp = val;
    }

private:
    temp_t sp;
};


// immutable SetPoint, always reading 'invalid' to indicate that the setpoint has not been configured
class SetPointConstant: public SetPoint {
public:
    SetPointConstant(const temp_t val): sp(val){}
    ~SetPointConstant(){};
    virtual temp_t read() const{
        return sp;
    }
    virtual void write(temp_t val){
    }

private:
    const temp_t sp;
};
