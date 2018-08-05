#include "Object.h"
#include "Container.h"
#include "EepromObjectStorage.h"
#include "Box.h"
#include <memory>

std::shared_ptr<cbox::Object> createApplicationObject(cbox::obj_type_t typeId, cbox::DataIn& in, cbox::CommandError& errorCode);

extern cbox::ObjectContainer objects;
extern cbox::EepromObjectStorage objectStore;
extern cbox::ConnectionPool connections;
extern cbox::Box appBox; 