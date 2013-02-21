/*
 * config.h
 *
 * Created: 21/02/2013 02:26:36
 *  Author: mat
 * This is the default configuration. Developers may add to this file when new configurable options are available. End users configuring their own boards
 * should instead edit config_local.h
 */ 


#ifndef CONFIG_H_
#define CONFIG_H_

#include "config_local.h"

#define CONFIG_LCD_DISPLAY (new LcdDisplay(tempControl))
#define CONFIG_NULL_DISPLAY (new NullDisplay())

/*
 * BREWPI_CONFIG values. These provide some common pre-defined setups for the brewpi.
 *
 *   shield  the brewpi-shield. Inverted actuators, LCD display. 
 */
#define BREWPI_CONFIG_SHIELD	1

 /*   bare    bare-bones arduino. Regular actuators, no display. */  
#define BREWPI_CONFIG_BARE		2

#ifndef BREWPI_CONFIG
#define BREWPI_CONFIG BREWPI_CONFIG_SHIELD
#endif

#if BREWPI_CONFIG == BREWPI_CONFIG_SHIELD
	#define CONFIG_DISPLAY CONFIG_LCD_DISPLAY
	#define CONFIG_INVERT_ACTUATOR true
#endif

#if BREWPI_CONFIG == BREWPI_CONFIG_BARE
	#define CONFIG_DISPLAY CONFIG_NULL_DISPLAY
	#define CONFIG_INVERT_ACTUATOR false
#endif

#ifndef CONFIG_BEER_SENSOR
#define CONFIG_BEER_SENSOR (new TempSensor(beerSensorPin))
#endif
#ifndef CONFIG_FRIDGE_SENSOR	
#define CONFIG_FRIDGE_SENSOR (new TempSensor(fridgeSensorPin))
#endif
#ifndef CONFIG_HEATER
#define CONFIG_HEATER (new DigitalPinActuator(heatingPin, CONFIG_INVERT_ACTUATOR))
#endif
#ifndef CONFIG_COOLER
#define CONFIG_COOLER (new DigitalPinActuator(coolingPin, CONFIG_INVERT_ACTUATOR))
#endif
#ifndef CONFIG_DOOR
#define CONFIG_DOOR (new SensorDoor(*new DigitalPinSensor(doorPin, USE_INTERNAL_PULL_UP_RESISTORS, false)))
#endif

#define HeaterActuator DigitalPinActuator
#define CoolerActuator DigitalPinActuator

#define CONFIG_DEFUALT_TEMP_CONTROL (new TempControl(*CONFIG_HEATER, *CONFIG_COOLER, *CONFIG_DOOR, *CONFIG_FRIDGE_SENSOR, *CONFIG_BEER_SENSOR))

#define CONFIG_TEMP_CONTROL CONFIG_DEFUALT_TEMP_CONTROL

// ensure all config symbols are defined

#ifndef CONFIG_DISPLAY
#error CONFIG_DISPLAY not defined
#endif

#ifndef CONFIG_INVERT_ACTUATOR
#error CONFIG_INVERT_ACTUATOR not defined
#endif



#endif /* CONFIG_H_ */