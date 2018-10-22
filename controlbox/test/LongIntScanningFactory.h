#include "Object.h"
#include "ObjectContainer.h"
#include "ScanningFactory.h"
#include "TestObjects.h"
#include <memory>
#include <vector>

namespace cbox {

/**
 * Simple mock factory that emulates object discovery
 * Normally, a scanning factory will scan some type of communication bus
 * This factory just has a list of candidates. If a LongIntObject with that value doesn't exist, it creates it.
 */
class LongIntScanningFactory : public ScanningFactory {
private:
    std::vector<uint32_t> candidates = {0x11111111, 0x22222222, 0x33333333, 0x44444444, 0x55555555};
    std::vector<uint32_t>::const_iterator it;

public:
    LongIntScanningFactory(ObjectContainer& objects)
        : ScanningFactory(objects)
    {
        reset();
    }

    virtual ~LongIntScanningFactory() = default;

    virtual void reset() override final
    {
        it = candidates.cbegin();
    };
    virtual std::shared_ptr<Object> scan() override final
    {
        while (it != candidates.cend()) {
            bool found = false;
            uint32_t value = *it;
            ++it;
            for (auto existing = objectsRef.cbegin(); existing != objectsRef.cend(); ++existing) {
                LongIntObject* ptrIfCorrectType = reinterpret_cast<LongIntObject*>(existing->object()->implements(LongIntObject::staticTypeId()));
                if (ptrIfCorrectType == nullptr) {
                    continue; // not the right type, no match
                }
                if (ptrIfCorrectType->value() == value) {
                    found = true; // object with value already exists
                    break;
                }
            }
            if (!found) {
                // create new object
                return std::make_shared<LongIntObject>(value);
            }
        }
        return nullptr;
    };
};

} // end namespace cbox