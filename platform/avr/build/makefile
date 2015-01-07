
#
# Copyright 2013 BrewPi/Elco Jacobs.
# Copyright 2014 Matthew McGowan
#
# This file is part of BrewPi.
# 
# BrewPi is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# BrewPi is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with BrewPi.  If not, see <http://www.gnu.org/licenses/>.
#

# convert build parameters to defines

# simulator flag control by shield type - the simulator is a virtual shield
BREWPI_SIMULATOR = 0

ifeq ($(SHIELD),diy)
	BREWPI_STATIC_CONFIG = BREWPI_SHIELD_DIY
else ifeq ($(SHIELD),revA)
	BREWPI_STATIC_CONFIG = BREWPI_SHIELD_REV_A
else ifeq ($(SHIELD),revC)
	BREWPI_STATIC_CONFIG = BREWPI_SHIELD_REV_C
else ifeq ($(SHIELD),simulator)
	BREWPI_SIMULATOR = 1
endif

## build target directory organization and output files

# base name of output files
TARGET_NAME ?= brewpi-avr

# location of .cpp sources
SRC=../

# temporary .o and .d files
OBJ_DIR = tmp/

# location of artefacts to keep
OUTPUT_DIR ?= final/

# default locations for tools based on the toolchain bin dir
AVRGCC ?= $(TOOLCHAIN_BIN)avr-g++$(EXE)
AVROBJCOPY ?= $(TOOLCHAIN_BIN)avr-objcopy$(EXE)
AVROBJDUMP ?= $(TOOLCHAIN_BIN)avr-objdump$(EXE)
AVRSIZE ?= $(TOOLCHAIN_BIN)avr-size$(EXE)

ARDUINO_VERSION ?= 105

# default config if nothing else specified
BOARD ?= leonardo
BREWPI_STATIC_CONFIG ?= BREWPI_SHIELD_REV_C
BREWPI_DEBUG ?= 0
BUILD_NUMBER ?= 0
BUILD_NAME ?= 00000000

DEFINES = \
 -DBREWPI_STATIC_CONFIG=$(BREWPI_STATIC_CONFIG) \
 -DBREWPI_DEBUG=$(BREWPI_DEBUG) \
 -DBREWPI_SIMULTE=$(BREWPI_SIMULATOR) \
 -DARDUINO=$(ARDUINO_VERSION) \
 -DF_CPU=16000000L \
 -DBUILD_NUMBER=$(BUILD_NUMBER) \
 -DBUILD_NAME="\"$(BUILD_NAME)"\"
 
ifeq ($(BOARD),leonardo)
	MCU = atmega32u4
	MCU_HEADERS = leonardo
else ifeq ($(BOARD),uno)
	MCU = atmega328p
	MCU_HEADERS = standard
endif

ARDUINO_HEADERS ?= $(SRC)arduino/cores/arduino

MCU_INCLUDE_VARIANT ?= $(SRC)arduino/variants
MCU_INCLUDE ?= $(MCU_INCLUDE_VARIANT)/$(MCU_HEADERS)

USER_OBJS :=

LIBS := 
PROJ := 

O_SRCS := 
C_SRCS := 
S_SRCS := 
S_UPPER_SRCS := 
OBJ_SRCS := 
ASM_SRCS := 
PREPROCESSING_SRCS := 
OBJS := 
OBJS_AS_ARGS := 
C_DEPS := 
C_DEPS_AS_ARGS := 
EXECUTABLES := 
OUTPUT_FILE_PATH :=
OUTPUT_FILE_PATH_AS_ARGS :=
AVR_APP_PATH :=$$$AVR_APP_PATH$$$
QUOTE := "
ADDITIONAL_DEPENDENCIES:=
OUTPUT_FILE_DEP:=
LIB_DEP:=

# Every subdirectory with source files must be described here
SUBDIRS :=  \
$(SRC)fallback


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
$(SRC)Actuator.cpp \
$(SRC)ActuatorArduino.cpp \
$(SRC)ArduinoFunctions.cpp \
$(SRC)Brewpi.cpp \
$(SRC)BrewpiStrings.cpp \
$(SRC)Buzzer.cpp \
$(SRC)DallasTemperature.cpp \
$(SRC)DeviceManager.cpp \
$(SRC)Display.cpp \
$(SRC)DisplayLcd.cpp \
$(SRC)DS2413.cpp \
$(SRC)EepromManager.cpp \
$(SRC)FilterCascaded.cpp \
$(SRC)FilterFixed.cpp \
$(SRC)Logger.cpp \
$(SRC)Main.cpp \
$(SRC)Menu.cpp \
$(SRC)OLEDFourBit.cpp \
$(SRC)OneWire.cpp \
$(SRC)OneWireTempSensor.cpp \
$(SRC)PiLink.cpp \
$(SRC)Random.cpp \
$(SRC)RotaryEncoder.cpp \
$(SRC)Sensor.cpp \
$(SRC)SettingsManager.cpp \
$(SRC)Simulator.cpp \
$(SRC)SpiLcd.cpp \
$(SRC)TempControl.cpp \
$(SRC)TemperatureFormats.cpp \
$(SRC)TempSensor.cpp \
$(SRC)Ticks.cpp


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
$(OBJ_DIR)Actuator.o \
$(OBJ_DIR)ActuatorArduinoPin.o \
$(OBJ_DIR)ArduinoFunctions.o \
$(OBJ_DIR)Brewpi.o \
$(OBJ_DIR)BrewpiStrings.o \
$(OBJ_DIR)Buzzer.o \
$(OBJ_DIR)DallasTemperature.o \
$(OBJ_DIR)DeviceManager.o \
$(OBJ_DIR)Display.o \
$(OBJ_DIR)DisplayLcd.o \
$(OBJ_DIR)DS2413.o \
$(OBJ_DIR)EepromManager.o \
$(OBJ_DIR)FilterCascaded.o \
$(OBJ_DIR)FilterFixed.o \
$(OBJ_DIR)Logger.o \
$(OBJ_DIR)Main.o \
$(OBJ_DIR)Menu.o \
$(OBJ_DIR)OLEDFourBit.o \
$(OBJ_DIR)OneWire.o \
$(OBJ_DIR)OneWireTempSensor.o \
$(OBJ_DIR)PiLink.o \
$(OBJ_DIR)Random.o \
$(OBJ_DIR)RotaryEncoder.o \
$(OBJ_DIR)Sensor.o \
$(OBJ_DIR)SettingsManager.o \
$(OBJ_DIR)Simulator.o \
$(OBJ_DIR)SpiLcd.o \
$(OBJ_DIR)TempControl.o \
$(OBJ_DIR)TemperatureFormats.o \
$(OBJ_DIR)TempSensor.o \
$(OBJ_DIR)Ticks.o

OBJS_AS_ARGS +=  \
$(OBJ_DIR)Actuator.o \
$(OBJ_DIR)ActuatorArduinoPin.o \
$(OBJ_DIR)ArduinoFunctions.o \
$(OBJ_DIR)Brewpi.o \
$(OBJ_DIR)BrewpiStrings.o \
$(OBJ_DIR)Buzzer.o \
$(OBJ_DIR)DallasTemperature.o \
$(OBJ_DIR)DeviceManager.o \
$(OBJ_DIR)Display.o \
$(OBJ_DIR)DisplayLcd.o \
$(OBJ_DIR)DS2413.o \
$(OBJ_DIR)EepromManager.o \
$(OBJ_DIR)FilterCascaded.o \
$(OBJ_DIR)FilterFixed.o \
$(OBJ_DIR)Logger.o \
$(OBJ_DIR)Main.o \
$(OBJ_DIR)Menu.o \
$(OBJ_DIR)OLEDFourBit.o \
$(OBJ_DIR)OneWire.o \
$(OBJ_DIR)OneWireTempSensor.o \
$(OBJ_DIR)PiLink.o \
$(OBJ_DIR)Random.o \
$(OBJ_DIR)RotaryEncoder.o \
$(OBJ_DIR)Sensor.o \
$(OBJ_DIR)SettingsManager.o \
$(OBJ_DIR)Simulator.o \
$(OBJ_DIR)SpiLcd.o \
$(OBJ_DIR)TempControl.o \
$(OBJ_DIR)TemperatureFormats.o \
$(OBJ_DIR)TempSensor.o \
$(OBJ_DIR)Ticks.o

C_DEPS +=  \
$(OBJ_DIR)Actuator.d \
$(OBJ_DIR)ActuatorArduinoPin.d \
$(OBJ_DIR)ArduinoFunctions.d \
$(OBJ_DIR)Brewpi.d \
$(OBJ_DIR)BrewpiStrings.d \
$(OBJ_DIR)Buzzer.d \
$(OBJ_DIR)DallasTemperature.d \
$(OBJ_DIR)DeviceManager.d \
$(OBJ_DIR)Display.d \
$(OBJ_DIR)DisplayLcd.d \
$(OBJ_DIR)DS2413.d \
$(OBJ_DIR)EepromManager.d \
$(OBJ_DIR)FilterCascaded.d \
$(OBJ_DIR)FilterFixed.d \
$(OBJ_DIR)Logger.d \
$(OBJ_DIR)Main.d \
$(OBJ_DIR)Menu.d \
$(OBJ_DIR)OLEDFourBit.d \
$(OBJ_DIR)OneWire.d \
$(OBJ_DIR)OneWireTempSensor.d \
$(OBJ_DIR)PiLink.d \
$(OBJ_DIR)Random.d \
$(OBJ_DIR)RotaryEncoder.d \
$(OBJ_DIR)Sensor.d \
$(OBJ_DIR)SettingsManager.d \
$(OBJ_DIR)Simulator.d \
$(OBJ_DIR)SpiLcd.d \
$(OBJ_DIR)TempControl.d \
$(OBJ_DIR)TemperatureFormats.d \
$(OBJ_DIR)TempSensor.d \
$(OBJ_DIR)Ticks.d

C_DEPS_AS_ARGS +=  \
$(OBJ_DIR)Actuator.d \
$(OBJ_DIR)ActuatorArduinoPin.d \
$(OBJ_DIR)ArduinoFunctions.d \
$(OBJ_DIR)Brewpi.d \
$(OBJ_DIR)BrewpiStrings.d \
$(OBJ_DIR)Buzzer.d \
$(OBJ_DIR)DallasTemperature.d \
$(OBJ_DIR)DeviceManager.d \
$(OBJ_DIR)Display.d \
$(OBJ_DIR)DisplayLcd.d \
$(OBJ_DIR)DS2413.d \
$(OBJ_DIR)EepromManager.d \
$(OBJ_DIR)FilterCascaded.d \
$(OBJ_DIR)FilterFixed.d \
$(OBJ_DIR)Logger.d \
$(OBJ_DIR)Main.d \
$(OBJ_DIR)Menu.d \
$(OBJ_DIR)OLEDFourBit.d \
$(OBJ_DIR)OneWire.d \
$(OBJ_DIR)OneWireTempSensor.d \
$(OBJ_DIR)PiLink.d \
$(OBJ_DIR)Random.d \
$(OBJ_DIR)RotaryEncoder.d \
$(OBJ_DIR)Sensor.d \
$(OBJ_DIR)SettingsManager.d \
$(OBJ_DIR)Simulator.d \
$(OBJ_DIR)SpiLcd.d \
$(OBJ_DIR)TempControl.d \
$(OBJ_DIR)TemperatureFormats.d \
$(OBJ_DIR)TempSensor.d \
$(OBJ_DIR)Ticks.d

OUTPUT_FILE_PATH +=$(OUTPUT_DIR)$(TARGET_NAME).elf

OUTPUT_FILE_PATH_AS_ARGS +=$(OUTPUT_DIR)$(TARGET_NAME).elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+= 

# AVR32/GNU C Compiler


#
# Recipes
#

./$(OBJ_DIR)%.o: $(SRC)./%.cpp
	@echo Building file: $<
	@echo Invoking: AVR8/GNU C Compiler
	"$(AVRGCC)" $(DEFINES) -mcall-prologues -funsigned-char -funsigned-bitfields -I"$(ARDUINO_HEADERS)" -I"$(MCU_INCLUDE)" -I"$(SRC)/fallback"  -Os -fno-split-wide-types -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -g3 -Wall -mmcu=$(MCU) -c -fno-exceptions -ffreestanding -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$(OBJ_DIR)" "$<"
	@echo Finished building: $<
	
# AVR32/GNU Preprocessing Assembler

# AVR32/GNU Assembler

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(OUTPUT_FILE_PATH) $(ADDITIONAL_DEPENDENCIES)

$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP) $(LIB_DEP)
	@echo Building target: $@
	@echo Invoking: AVR8/GNU Linker
	"$(AVRGCC)" -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -Wl,-Map="$(OUTPUT_DIR)$(TARGET_NAME).map" -Wl,--start-group -Wl,-lm  -Wl,--end-group -Wl,--gc-sections -mmcu=$(MCU) -Wl,--relax 
	@echo Finished building target: $@
	"$(AVROBJCOPY)" -O ihex -R .eeprom -R .fuse -R .lock -R .signature  "$(OUTPUT_DIR)$(TARGET_NAME).elf" "$(OUTPUT_DIR)$(TARGET_NAME).hex"
	
	"$(AVROBJCOPY)" -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex "$(OUTPUT_DIR)$(TARGET_NAME).elf" "$(OUTPUT_DIR)$(TARGET_NAME).eep" || exit 0
	
	"$(AVROBJDUMP)" -h -S "$(OUTPUT_DIR)$(TARGET_NAME).elf" > "$(OUTPUT_DIR)$(TARGET_NAME).lss"
	
	"$(AVROBJCOPY)" -O srec -R .eeprom -R .fuse -R .lock -R .signature  "$(OUTPUT_DIR)$(TARGET_NAME).elf" "$(OUTPUT_DIR)$(TARGET_NAME).srec"
	
	"$(AVRSIZE)" "$(OUTPUT_DIR)$(TARGET_NAME).elf" > "$(OUTPUT_DIR)$(TARGET_NAME).elf.size"

	


# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	-$(RM) "$(OUTPUT_DIR)$(TARGET_NAME).elf" "$(OUTPUT_DIR)$(TARGET_NAME).a" "$(OUTPUT_DIR)$(TARGET_NAME).hex" "$(OUTPUT_DIR)$(TARGET_NAME).lss" "$(OUTPUT_DIR)$(TARGET_NAME).eep" "$(OUTPUT_DIR)$(TARGET_NAME).map" "$(OUTPUT_DIR)$(TARGET_NAME).srec"
