/*
 * ConfigLocal.h
 *
 * Created: 21/02/2013 02:34:18
 *  Author: mat
 *
 * Edit this file to make local changes. This should be committed to git as an ignored empty file.
 */ 

#ifndef CONFIG_LOCAL_H_
#define CONFIG_LOCAL_H_

#define BREWPI_SIMULATE 0
#define BREWPI_EMULATE 0
#define BREWPI_DEBUG 1

#define BREWPI_STATIC_CONFIG BREWPI_SHIELD_REV_C

#define  BREWPI_DS2413 0


// for some reason, my shield on Uno doesn't see the A4 onewire bus, yet this works fine on my Mega2560.
// So, I make the default bus A5.
//#define oneWirePin A5
//#define actuatorPin4 A4

// I'm using onewire for all actuators
// may use the door sensor so will keep that.
//#define BREWPI_ACTUATOR_PINS 0



#endif /* CONFIG_LOCAL_H_ */