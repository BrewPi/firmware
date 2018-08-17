#include "Object.h"
#include "Container.h"
#include "EepromObjectStorage.h"
#include "Box.h"
#include "OneWire.h"
#include <memory>
#include "ObjectFactory.h"

extern cbox::ObjectContainer objects;
extern cbox::EepromObjectStorage objectStore;
extern cbox::ConnectionPool connections;
extern cbox::Box appBox; 
extern OneWire oneWireBus;
