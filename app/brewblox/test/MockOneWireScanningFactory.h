#include "OneWire.h"
#include "OneWireAddress.h"
#include "OneWireDevice.h"
#include "blox/TempSensorOneWireBlock.h"
#include "cbox/Object.h"
#include "cbox/ObjectContainer.h"
#include "cbox/ScanningFactory.h"
#include <memory>
#include <vector>

/**
 * Simple mock factory that emulates object discovery
 * Normally, a scanning factory will scan some type of communication bus
 * This factory just has a list of candidates. If a LongIntObject with that value doesn't exist, it creates it.
 */
class MockOneWireScanningFactory : public cbox::ScanningFactory {
private:
    OneWire& bus;
    std::vector<OneWireAddress> adressesOnBus = {0x1111'1111'1111'1128, 0x2222'2222'2222'2228, 0x3333'3333'3333'3328, 0x1111'1111'1111'1128};
    std::vector<OneWireAddress>::const_iterator nextAddress;

public:
    MockOneWireScanningFactory(cbox::ObjectContainer& objects, OneWire& ow)
        : cbox::ScanningFactory(objects)
        , bus(ow)
    {
        reset();
    }

    virtual ~MockOneWireScanningFactory() = default;

    virtual void reset() override final
    {
        nextAddress = adressesOnBus.cbegin();
    };
    virtual std::shared_ptr<cbox::Object> scan() override final
    {
        while (true) {
            if (nextAddress != adressesOnBus.cend()) {
                bool found = false;
                auto newAddr = *nextAddress;
                ++nextAddress;
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
