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
#include "ControllerMixins.h"

class SetPoint : public SetPointMixin{
public:
    SetPoint(){};
    virtual ~SetPoint(){};
    virtual temp_t read() const = 0;
    virtual void write(temp_t val) = 0;
friend class SetPointMixin;
};


class SetPointSimple : public SetPoint, public SetPointSimpleMixin {
public:
    SetPointSimple(temp_t val = temp_t::disabled()) : value(val){}
    virtual ~SetPointSimple(){};
    virtual temp_t read() const{
        return value;
    }
    virtual void write(temp_t val){
        value = val;
    }

private:
    temp_t value;
friend class SetPointSimpleMixin;
};


// immutable SetPoint, always reading for example 'invalid' to indicate that the setpoint has not been configured
class SetPointConstant: public SetPoint, public SetPointConstantMixin {
public:
    SetPointConstant(const temp_t val): value(val){}
    ~SetPointConstant(){};
    temp_t read() const{
        return value;
    }
    void write(temp_t val){
    }

private:
    const temp_t value;

friend class SetPointConstantMixin;
};

class SetPointNamed : public SetPoint, public SetPointNamedMixin {
public:
    SetPointNamed(SetPoint * sp) : setPoint(sp){}
    ~SetPointNamed(){}

    inline temp_t read() const{
        return setPoint->read();
    }
    inline void write(temp_t val){
        setPoint->write(val);
    }

private:
    SetPoint * setPoint;

friend class SetPointNamedMixin;
};
