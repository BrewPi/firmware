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
#include "TempSensorInterface.h"
#include "ActuatorInterfaces.h"

template<class T>
T* defaultTarget();

template<class T>
T* asInterface(Interface* i);


// explicit instantiation of template functions is done in cpp file.
// declare extern here to prevent circular include errors
extern template ActuatorDigitalInterface* asInterface<ActuatorDigitalInterface>(Interface*);
extern template ActuatorRangeInterface* asInterface<ActuatorRangeInterface>(Interface*);
extern template TempSensorInterface* asInterface<TempSensorInterface>(Interface*);
extern template SetPointInterface* asInterface<SetPointInterface>(Interface*);

template<class T>
class RefTo {
public:
    RefTo(){};
    RefTo(std::function<Interface* ()> lookup) : lookup(std::move(lookup)){};
    ~RefTo() = default;

    void setLookup(std::function<Interface* ()> newLookup){
        lookup = newLookup;
    }
    std::function<Interface* ()>  getLookup(){
        return lookup;
    }

    T* get(){
        T* specializedTarget = nullptr;
        if(lookup){
            Interface* target = lookup();
            if(target){
                specializedTarget = asInterface<T>(target);
            }
        }
        return (specializedTarget) ? specializedTarget : defaultTarget<T>();
    }

    T& operator()() {
        return *get();
    }

private:
    // callable loopup object, must implement () operator
    // and hold information required for lookup
    std::function<Interface* ()> lookup;
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
