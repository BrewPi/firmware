/*
* Copyright 2016 BrewPi/Elco Jacobs.
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

#include "SmartPtrContainerDemo.h"

// macro to easily output a data member behind a smart pointer, PE is with trailing comma, PT without
#define JSON_PE(json_writer,class_member)  JSON::stream(json_writer,_ASTRING(#class_member),get()->class_member,true)
#define JSON_PT(json_writer,class_member)  JSON::stream(json_writer,_ASTRING(#class_member),get()->class_member,false)


uint16_t getContainerId(){
    static uint16_t counter = 0;
    return counter ++;
}

template<>
void SmartPtrContainer<TestTarget>::serialize(JSON::Adapter & adapter){
    JSON::Class root(adapter, "TestTarget");
    serializeHeader(adapter);
    JSON_PT(adapter, b);
}

template<>
void SmartPtrContainer<TestDriver>::serialize(JSON::Adapter & adapter){
    JSON::Class root(adapter, "TestDriver");
    serializeHeader(adapter);
    JSON_PE(adapter, a);
    JSON_PE(adapter, target);
    int16_t ab = get()->calc();
    JSON_T(adapter, ab);
}


