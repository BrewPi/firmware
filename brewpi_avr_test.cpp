#include <Arduino.h>

#include "Ticks.h"
#include "LcdDisplay.h"
#include "TempControl.h"
#include "Menu.h"
#include "pins.h"
#include "RotaryEncoder.h"
#include "Buzzer.h"
#include "chamber.h"
#include "TempSensor.h"
#include "MockTempSensor.h"
#include "OneWireTempSensor.h"
#include "Ticks.h"
#include "brewpi_avr.h"
#include "config.h"
#include "Sensor.h"

#include "ArduinoUnit.h"
#include "PiLink.h"
#include "utility/FakeSerial.h"


// with a test build, piLink uses the external SerialCapture reference so we can capture it's output
FakeStream fake;
FakeStream& SerialCapture(fake);

// test reporting goes to the real Serial output when running live
// under emulation, use in-memory serial output so we can view at the end of the test run
#if BREWPI_EMULATE
FakeSerial TestReport;

#endif


void setup()
{
	// for testing under the emulator, place output in a buffer rather than writing to serial
	#if BREWPI_EMULATE
	Test::out = &TestReport;
	#endif

	
	Test::min_verbosity = Test::max_verbosity = TEST_VERBOSITY_TESTS_FAILED | TEST_VERBOSITY_ASSERTIONS_FAILED;
}

void loop()
{
	Test::run();
	const String& report = TestReport.bytesWritten();
	
}

class PiLinkTest {
	// put these test functions in a class, makes for easier grouping and also provides private access
public:	
	static void initSendJsonPair()
	{
		fake.clearBytesWritten();
		piLink.firstPair = true;
	}

	static void sendJsonPair_string_first()
	{
		initSendJsonPair();
		piLink.sendJsonPair_P(PSTR("abc"), "def");
		const String& result = fake.bytesWritten();
		String expected = "{\"abc\":def";
		assertEqual(result, expected);
	}

	static void sendJsonPair_string_subsequent()
	{
		initSendJsonPair();
		piLink.firstPair = false;
		piLink.sendJsonPair_P(PSTR("abc"), "def");
		const String& result = fake.bytesWritten();
		String expected = ",\"abc\":def";
		assertEqual(result, expected);
	}

	static void sendJsonPair_string_last()
	{
		initSendJsonPair();
		piLink.firstPair = false;
		piLink.sendJsonPair_P(PSTR("abc"), "def");
		piLink.sendJsonClose();
		const String& result = fake.bytesWritten();
		String expected = ",\"abc\":def}";
		assertEqual(result, expected);
	}

	static void sendJsonPair_uint8_first()
	{
		initSendJsonPair();
		piLink.sendJsonPair_P(PSTR("abc"), (uint8_t)14);
		const String& result = fake.bytesWritten();
		String expected = "{\"abc\": \"14\"";
		assertEqual(result, expected);
	}

	static void sendJsonPair_uint8_subsequent()
	{
		initSendJsonPair();
		piLink.firstPair = false;
		piLink.sendJsonPair_P(PSTR("abc"), (uint8_t)14);
		const String& result = fake.bytesWritten();
		String expected = ",\"abc\": \"14\"";
		assertEqual(result, expected);
	}

	static void sendJsonPair_uint8_last()
	{
		initSendJsonPair();
		piLink.firstPair = false;
		piLink.sendJsonPair_P(PSTR("abc"), (uint8_t)14);
		piLink.sendJsonClose();
		const String& result = fake.bytesWritten();
		String expected = ",\"abc\": \"14\"}";
		assertEqual(result, expected);
	}
	
};

#define PiLinkTest(name) test(name) { PiLinkTest::name(); }

PiLinkTest(sendJsonPair_string_first)
PiLinkTest(sendJsonPair_string_subsequent)
PiLinkTest(sendJsonPair_string_last)

PiLinkTest(sendJsonPair_uint8_first)
PiLinkTest(sendJsonPair_uint8_subsequent)
PiLinkTest(sendJsonPair_uint8_last)

