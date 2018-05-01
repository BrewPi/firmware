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

// simple lookup class that just keeps a pointer to its target
class BaseLookup {
public:
    BaseLookup(){}
    ~BaseLookup() = default;

    virtual Interface * operator()() const = 0;
};


class RefToGeneric {
public:
    RefToGeneric(BaseLookup const& lookup) : lookup(lookup){};
    ~RefToGeneric() = default;

    Interface* get() const {
    	return lookup();
    }
    Interface& operator()() const {
        return *get();
    }

private:
    // callable loopup object reference, must implement () operator
    // and hold information required for lookup
    BaseLookup const& lookup;
};

template<class T>
class RefTo : public RefToGeneric {
public:
    RefTo(BaseLookup const& lookup) : RefToGeneric(lookup){};
    ~RefTo() = default;


    T* get() const {
        T* specializedTarget = nullptr;
        Interface * target = RefToGeneric::get();
        if(target != nullptr){
            specializedTarget = asInterface<T>(target);
        }
        return (specializedTarget) ? specializedTarget : defaultTarget<T>();
    }

    T& operator()() const {
        return *get();
    }
};

// simple lookup class that just keeps a pointer to its target
class PtrLookup : public BaseLookup {
public:
    PtrLookup(Interface * target) : target(target) {}
    ~PtrLookup() = default;

    virtual Interface * operator()() const override final {
        return target;
    }

    void setPtr(Interface * target_){
        target = target_;
    }
private:
    Interface* target;
};
