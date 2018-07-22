#include <functional>
#include <stdint.h>
#include <vector>
#include "Object.h"
#include "ResolveType.h"

namespace cbox {

#if 0
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
    virtual std::shared_ptr<Object> fetch(const object_id_t id) = 0;
    virtual object_id_t add (std::unique_ptr<Object> obj, uint8_t active_in_profiles) = 0;
    virtual bool replace (object_id_t id, std::unique_ptr<Object> obj, uint8_t active_in_profiles) = 0;
    virtual void remove(object_id_t id) = 0;
};
#endif

class Container
{
public:
    Container() : activeProfiles(0), lastCreatedId(object_id_t::startId()){};
    virtual ~Container() = default;

    class ObjectEntry {
    public:
        ObjectEntry(object_id_t _id, uint8_t _profiles, std::shared_ptr<Object> _obj) :
            id(_id),
            profiles(_profiles),
            obj(_obj){};

        object_id_t id;
        uint8_t profiles; // active in these profiles
        std::shared_ptr<Object> obj; // ptr to runtime object
    };

private:
    std::vector<ObjectEntry> objects;
    uint8_t activeProfiles;
    object_id_t lastCreatedId;

public:

    /**
     * finds the object entry with the given id.
     * @return pointer to the entry.
     *
     */

    ObjectEntry * find(const object_id_t id) {
        decltype(objects)::iterator begin = objects.begin();
        decltype(objects)::iterator end = objects.end();
        decltype(objects)::iterator found = std::find_if(begin, end, [&id](const ObjectEntry& item){ return item.id == id;} );
        if(found == end){
            return nullptr;
        }
        return &(*found);
    }

    std::shared_ptr<Object> fetch(const object_id_t id) {
        auto entry = find(id);
        std::shared_ptr<Object> ptr;
        if(entry != nullptr){
            ptr = entry->obj;
        }
        return ptr;
    }

    object_id_t add (std::unique_ptr<Object> obj, uint8_t active_in_profiles) {
        object_id_t newId = lastCreatedId++;
        std::shared_ptr<Object> ptr = std::move(obj);
        ObjectEntry entry(newId, active_in_profiles, ptr);
        return newId;
    }

    bool replace (object_id_t id, std::unique_ptr<Object> obj, uint8_t active_in_profiles) {
        ObjectEntry * entry = find(id);
        if(entry != nullptr){
            entry->profiles = active_in_profiles;
            entry->obj = std::move(obj);
            return true;
        }
        return false;
    }

    void remove(object_id_t id) {
        std::remove_if(objects.begin(), objects.end(), [&id](ObjectEntry const& item){ return item.id == id;} );
    }

    void map(std::function<void(ObjectEntry & entry)> func) {
        for(auto it : objects){
            func(it);
        }
    }
};

} // end namespace cbox
