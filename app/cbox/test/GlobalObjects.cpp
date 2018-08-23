#include "theOneWire.h"
#include "OneWire.h"

OneWire& theOneWire(){
    static OneWire ow(0);
    return ow;
}
