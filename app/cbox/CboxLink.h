#pragma once

#include "RefTo.h"
#include "Values.h"
#include <cstring>

#define MAX_ID_CHAIN_LENGHT (MAX_CONTAINER_DEPTH+1)


class CboxLookup : public BaseLookup {
public:
    CboxLookup(){
    }
    virtual ~CboxLookup() = default;

    virtual Interface * operator()() const override final {
        // id_chain.reset();

        Object * root = &systemRootContainer();
        Object * current = root;
        container_id id = -1;
        uint8_t i = 0;
        while(i < MAX_ID_CHAIN_LENGHT && id < 0){
            id = id_chain[i++];
            current = fetchContainedObject(current, id);
        }
        return (current && current != root) ? current->getApplicationInterface() : nullptr;
    }

    // wire type is unsigned bytes
    void copyFrom(uint8_t const (&from)[MAX_ID_CHAIN_LENGHT]){
        memcpy(id_chain, from, MAX_ID_CHAIN_LENGHT);
    }

    void copyTo(uint8_t (&to)[MAX_ID_CHAIN_LENGHT]){
        memcpy(to, id_chain, MAX_ID_CHAIN_LENGHT);
    }

private:
    container_id id_chain[MAX_ID_CHAIN_LENGHT];
};
