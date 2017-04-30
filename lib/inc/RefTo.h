/*
 * Copyright 2015 Matthew McGowan
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

#include <stdint.h>
#include <functional>
#include "Interface.h"
#include "SetPoint.h"
#include "TempSensor.h"
#include "ActuatorInterfaces.h"

template<class T>
T* defaultTarget();

template<class T>
T* asInterface(Interface* i);


// explicit instantiation of template functions is done in cpp file.
// declare extern here to prevent circular include errors
extern template ActuatorDigital* asInterface<ActuatorDigital>(Interface*);
extern template ActuatorAnalog* asInterface<ActuatorAnalog>(Interface*);
extern template TempSensor* asInterface<TempSensor>(Interface*);
extern template SetPoint* asInterface<SetPoint>(Interface*);


class RefToGeneric {
public:
    RefToGeneric(){};
    RefToGeneric(std::function<Interface* ()> lookup) : lookup(std::move(lookup)){};
    ~RefToGeneric() = default;
    void setLookup(std::function<Interface* ()> newLookup){
        lookup = newLookup;
    }
    std::function<Interface* ()>  getLookup(){
        return lookup;
    }
    Interface* get() const {
        return lookup();
    }
    Interface& operator()() const {
        return *get();
    }

protected:
    // callable loopup object, must implement () operator
    // and hold information required for lookup
    std::function<Interface* ()> lookup;
};

template<class T>
class RefTo : public RefToGeneric {
public:
    RefTo(){};
    RefTo(std::function<Interface* ()> lookup) : RefToGeneric(lookup){};
    ~RefTo() = default;


    T* get() const {
        T* specializedTarget = nullptr;
        if(lookup){
            Interface* target = lookup();
            if(target){
                specializedTarget = asInterface<T>(target);
            }
        }
        return (specializedTarget) ? specializedTarget : defaultTarget<T>();
    }

    T& operator()() const {
        return *get();
    }
};

// simple lookup class that just keeps a pointer to its target
class PtrLookup {
public:
    PtrLookup(Interface * target) : target(target) {}
    ~PtrLookup() = default;

    Interface * operator()(){
        return target;
    }
private:
    Interface* target;
};
