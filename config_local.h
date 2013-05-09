/*
 * config_local.h
 *
 * Created: 21/02/2013 02:34:18
 *  Author: mat
 *
 * Edit this file to make local changes. This should be committed to git as an ignored empty file.
 */ 

#ifndef CONFIG_LOCAL_H_
#define CONFIG_LOCAL_H_

#define BREWPI_DEBUG 1
#define OPTIMIZE_JSON_OUTPUT 0

#define BREWPI_STATIC_CONFIG BREWPI_SHIELD_REV_A

#ifndef BREWPI_DS2413
#define  BREWPI_DS2413 1
#endif

// for some reason, my shield on Uno doesn't see the A4 onewire bus, yet this works fine on my Mega2560.
// So, I make the default bus A5.
#define oneWirePin A5
#define actuatorPin4 A4

#endif /* CONFIG_LOCAL_H_ */