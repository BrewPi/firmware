/**
 ******************************************************************************
 * @file    SparkEepromRegions.h
 * @authors mat
 * @date    19 February 2015
 ******************************************************************************
  Copyright (c) 2015 Brewpi, Inc.  All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation, either
  version 3 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************
 */

#ifndef SPARKEEPROMREGIONS_H
#define	SPARKEEPROMREGIONS_H

#include "EepromFormat.h"

#if PLATFORM_ID==0
#define EEPROM_CONTROLLER_START_BLOCK 0
#define EEPROM_CONTROLLER_END_BLOCK 32
#define EEPROM_EGUI_SETTINGS_START_BLOCK 32
#define EEPROM_EGUI_SETTINGS_END_BLOCK 64
#elif PLATFORM_ID==6
#define EEPROM_CONTROLLER_START_BLOCK 2
#define EEPROM_CONTROLLER_END_BLOCK (EEPROM_CONTROLLER_START_BLOCK + EepromFormat::MAX_EEPROM_SIZE)
#define EEPROM_EGUI_SETTINGS_START_BLOCK EEPROM_CONTROLLER_END_BLOCK
#define EEPROM_EGUI_SETTINGS_END_BLOCK EEPROM_CONTROLLER_END_BLOCK+64
#else
#error "Unknown platform ID"
#endif

#endif	/* SPARKEEPROMREGIONS_H */

