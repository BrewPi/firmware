#include "OneWire.h"
#include "OneWireAddress.h"
#include "OneWireDevice.h"
#include "blox/TempSensorOneWireBlock.h"
#include "cbox/Object.h"
#include "cbox/ObjectContainer.h"
#include "cbox/ScanningFactory.h"
#include <memory>

/**
 * Simple mock factory that emulates object discovery
 * Normally, a scanning factory will scan some type of communication bus
 * This factory just has a list of candidates. If a LongIntObject with that value doesn't exist, it creates it.
 */
class OneWireScanningFactory : public cbox::ScanningFactory {
private:
    OneWire& bus;

public:
    OneWireScanningFactory(cbox::ObjectContainer& objects, OneWire& ow)
        : cbox::ScanningFactory(objects)
        , bus(ow)
    {
        reset();
    }

    virtual ~OneWireScanningFactory() = default;

    virtual void reset() override final
    {
        bus.reset_search();
    };
    virtual std::shared_ptr<cbox::Object> scan() override final
    {
        auto newAddr = OneWireAddress();
        while (true) {
            if (bus.search(newAddr.asUint8ptr())) {
                bool found = false;
                for (auto existing = objectsRef.cbegin(); existing != objectsRef.cend(); ++existing) {
                    OneWireDevice* ptrIfCorrectType = reinterpret_cast<OneWireDevice*>(existing->object()->implements(cbox::interfaceId<OneWireDevice>()));
                    if (ptrIfCorrectType == nullptr) {
                        continue; // not the right type, no match
                    }
                    if (ptrIfCorrectType->getDeviceAddress() == newAddr) {
                        found = true; // object with value already exists
                        break;
                    }
                }
                if (!found) {
                    // create new object
                    uint8_t familyCode = newAddr.asUint8ptr()[0];
                    switch (familyCode) {
                    case DS18B20MODEL: {
                        auto newSensor = std::make_shared<TempSensorOneWireBlock>();
                        newSensor->get().setAddress(newAddr);
                        return std::move(newSensor);
                    }
                    default:
                        break;
                    }
                }
            } else {
                break;
            }
        };
        return nullptr;
    };
};
