#pragma once
#include "Values.h"
#include "OneWire.h"
#include "OneWireBusCBox.h"
#include "OneWireTempSensor.h"
#include <assert.h>

// since we only have one then might as well reference it directly
// this will change when support for multiple busses is added.
extern OneWireBusCBox oneWireBus;

class OneWireTempSensorCBox : public WritableValue {

	OneWireTempSensor sensor;

	OneWireTempSensorCBox(OneWire& bus, DeviceAddress& address, temp_t offset)
	: sensor(&bus, address, offset){
		sensor.init();
	}

public:

	virtual prepare_t prepare() override {
		// this works, but is kind of backwards, since the sensor caches the current value, and then
		// sends a conversion request. It would be clearer if there is a
		// separate "prepare" phase and then a fetch phase (which is the model expected here.)
		sensor.update();
		return 750;
	}

	 /**
	  * First byte is connected status,
	  * then 4 bytes is the temperature, 32-bit fixed point
	  * then optionally
	  * 2 bytes for the calibration offset
	  * and 8 bytes for the sensor address
	  * (and later) 1 byte for the USB bus id
	  */
	virtual void readTo(DataOut& out) override {
		bool connected = sensor.isConnected();
		out.write(connected ? 01 : 00);
		if (connected) {
			 temp_long_t value = sensor.read();
			 writePlatformEndianBytes(&value, sizeof(value), out);
		}
		 // we don't return the address, that can be found out from the object definition
		 // and the same is true for the calibration offset
	}

	virtual uint8_t readStreamSize() override {
		return 0;
	}

	virtual void writeMaskedFrom(DataIn& dataIn, DataIn& maskIn) override {
		// currently a no-op - will later allow the calibration and the address to be updated (and persisted)
	}

	/**
	 * 8 bytes for the device address
	 * 2 bytes for the temperature offset that is added to readings
	 */
	static Object* create(ObjectDefinition& defn) {
		DeviceAddress address;
		temp_t offset;
		static_assert(sizeof(offset)==2, "expected temp_t to be 2 bytes");
		defn.in->read(address, sizeof(address));
		DefaultMask mask;	// todo - make a utility function that reads without a mask
		readPlatformEndianMaskedBytes(&offset, sizeof(offset), *defn.in, mask);
		return new_object(OneWireTempSensorCBox(oneWireBus.oneWire(), address, offset));
	}
};
