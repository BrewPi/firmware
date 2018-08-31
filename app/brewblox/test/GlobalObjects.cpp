#include "OneWire.h"
#include "OneWireNull.h"
#include "theOneWire.h"

OneWire&
theOneWire()
{
    static OneWireNull driver(0);
    static OneWire ow(driver);
    return ow;
}
