#include <functional>
#include <stdint.h>
#include "Object.h"
#include "ResolveType.h"

namespace cbox {

/**
 * A container that holds items.
 */
class Container
{
public:
    Container() = default;
    virtual ~Container() = default;

    /**
     * Fetches the object with the given id.
     * /return the object with that id, which may be null.
     *
     * After retrieving the item, callers must call returnItem()
     */
    virtual std::shared_ptr<Object> fetch(object_id_t id) = 0;
    virtual object_id_t add (std::unique_ptr<Object> obj, uint8_t active_in_profiles) = 0;
    virtual bool replace (object_id_t id, std::unique_ptr<Object> obj, uint8_t active_in_profiles) = 0;
    virtual void remove(object_id_t id) = 0;
    virtual void map(std::function<void(Object * obj)>) = 0; // call a function on all objects in the container
};


class ProfileAwareContainer
{
public:
    ProfileAwareContainer() : activeProfiles(0), lastCreatedId(object_id_t::startId()){};
    virtual ~ProfileAwareContainer() = default;

    class ObjectEntry {
        ObjectEntry(object_id_t _id, uint8_t _profiles, Object * _obj) :
            id(_id),
            profiles(_profiles),
            obj(_obj){};

        object_id_t id;
        uint8_t profiles; // active in these profiles
        std::shared_ptr<Object> obj; // ptr to runtime object
    };

private:
    std::vector<ObjectEntry> objects;
    object_id_t lastCreatedId;
    uint8_t activeProfiles;

public:

    /**
     * finds the object entry with the given id.
     * @return pointer to the entry.
     *
     */
    ObjectEntry * find(object_id_t id) override final {
        auto found = std::find_if(objects.begin(), objects.end(), [&id](ObjectEntry const& item){ return item.id == id;} );
        if(found == objects.end()){
            found = nullptr;
        }
        return found;
    }

    virtual std::shared_ptr<Object> fetch(object_id_t id){
        auto entry = find(id);
        std::shared_ptr<Object> ptr;
        if(entry != nullptr){
            ptr = entry->obj;
        }
        return ptr;
    }

    virtual object_id_t add (std::unique_ptr<Object> obj, uint8_t active_in_profiles) override final {
        object_id_t newId = lastCreatedId++;
        std::shared_ptr<Object> ptr = std::move(obj);
        ObjectEntry entry(newId, active_in_profiles, ptr);
        return newId;
    }

    virtual bool replace (object_id_t id, std::unique_ptr<Object> obj, uint8_t active_in_profiles) override final {
        ObjectEntry * entry = find(id);
        if(entry != nullptr){
            entry->profiles = active_in_profiles;
            entry->obj = obj;
            return true;
        }
        return false;
    }

    virtual void remove(object_id_t id) override final{
        std::remove_if(objects.begin(), objects.end(), [&id](ObjectEntry const& item){ return item.id == id;} );
    }

    virtual void map(std::function<void(Object * obj)> func) override final {
        for(auto it = objects.begin(); it < objects.end(); objects++){
            func(it);
        }
    }
};

} // end namespace cbox
