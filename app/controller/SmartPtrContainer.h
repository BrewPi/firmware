#pragma once

#include <memory>
#include "json_writer.h"

// Some container manager dispatches IDs
uint16_t getContainerId();

// generic part for every container
class SmartPtrContainerBase
{
public:
    SmartPtrContainerBase(){
        id = getContainerId();
    };
    virtual ~SmartPtrContainerBase(){
        // search other containers for references to me (by unique ID) and invalidate these
        // by setting the reference to default containers
    }

    virtual void serialize(JSON::Adapter & adapter) = 0;

    // some generic variables in each container
    uint16_t id;

    // serializing these generic variables
    void serializeHeader(JSON::Adapter & adapter) {
        uint16_t id = this->id; // quick hack to work around ESJ template resolution failing with const vars
        JSON_E(adapter, id);
    }
};

// templated container part to wrap specific class
template<class T>
class SmartPtrContainer : public SmartPtrContainerBase, public std::unique_ptr<T>{
public:
    using std::unique_ptr<T>::unique_ptr;
    virtual ~SmartPtrContainer() = default;

    virtual void serialize(JSON::Adapter & adapter) final;
};
