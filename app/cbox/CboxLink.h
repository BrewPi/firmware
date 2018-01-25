#include "RefTo.h"
#include "Values.h"

class CboxLookup : public BaseLookup {
public:
    CboxLookup(){
    }
    ~CboxLookup() = default;

    virtual Interface * operator()() const override final {
        // id_chain.reset();

        Object * root = &systemRootContainer();
        Object * current = root;
        container_id id = -1;
        uint8_t i = 0;
        while(i < 4 && id < 0){
            id = id_chain[i++];
            current = fetchContainedObject(current, id);
        }
        return (current && current != root) ? current->getApplicationInterface() : nullptr;
    }

    void copyFrom(void * from){
        memcpy(id_chain, from, 4);
    }

    void copyTo(void * to){
        memcpy(to, id_chain, 4);
    }

private:
    container_id id_chain[4];
};
