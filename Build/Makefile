RM = del 
TOOLCHAIN_BIN ?= C:/Program Files (x86)/Atmel/Atmel Toolchain/AVR8 GCC/Native/3.4.2.939/avr8-gnu-toolchain/bin/

OUTPUT_DIR ?=

AVRGCC ?= $(TOOLCHAIN_BIN)avr-g++.exe
AVROBJCOPY ?= $(TOOLCHAIN_BIN)avr-objcopy.exe
AVROBJDUMP ?= $(TOOLCHAIN_BIN)avr-objdump.exe
AVRSIZE ?= $(TOOLCHAIN_BIN)avr-size.exe

ARDUINO ?= 105

BOARD ?= leonardo

ifeq ($(BOARD),leonardo)
	MCU = atmega32u4
	MCU_HEADERS = leonardo
else ifeq ($(BOARD),uno)
	MCU = atmega328p
	MCU_HEADERS = standard
endif

MCU_INCLUDE_VARIANT ?= $(ARDUINO_HOME)\hardware\arduino\variants
ARDUINO_HEADERS ?= $(ARDUINO_HOME)\hardware\arduino\cores\arduino
MCU_INCLUDE ?= $(MCU_INCLUDE_VARIANT)\$(MCU_HEADERS)



RM ?= rm -rf

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
../fallback


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../Actuator.cpp \
../ArduinoFunctions.cpp \
../Brewpi.cpp \
../BrewpiStrings.cpp \
../Buzzer.cpp \
../DallasTemperature.cpp \
../DeviceManager.cpp \
../Display.cpp \
../DisplayLcd.cpp \
../DS2413.cpp \
../EepromManager.cpp \
../FilterCascaded.cpp \
../FilterFixed.cpp \
../Logger.cpp \
../Main.cpp \
../Menu.cpp \
../OLEDFourBit.cpp \
../OneWire.cpp \
../OneWireTempSensor.cpp \
../PiLink.cpp \
../Random.cpp \
../RotaryEncoder.cpp \
../Sensor.cpp \
../SettingsManager.cpp \
../Simulator.cpp \
../SpiLcd.cpp \
../TempControl.cpp \
../TemperatureFormats.cpp \
../TempSensor.cpp \
../Ticks.cpp


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
Actuator.o \
ArduinoFunctions.o \
Brewpi.o \
BrewpiStrings.o \
Buzzer.o \
DallasTemperature.o \
DeviceManager.o \
Display.o \
DisplayLcd.o \
DS2413.o \
EepromManager.o \
FilterCascaded.o \
FilterFixed.o \
Logger.o \
Main.o \
Menu.o \
OLEDFourBit.o \
OneWire.o \
OneWireTempSensor.o \
PiLink.o \
Random.o \
RotaryEncoder.o \
Sensor.o \
SettingsManager.o \
Simulator.o \
SpiLcd.o \
TempControl.o \
TemperatureFormats.o \
TempSensor.o \
Ticks.o

OBJS_AS_ARGS +=  \
Actuator.o \
ArduinoFunctions.o \
Brewpi.o \
BrewpiStrings.o \
Buzzer.o \
DallasTemperature.o \
DeviceManager.o \
Display.o \
DisplayLcd.o \
DS2413.o \
EepromManager.o \
FilterCascaded.o \
FilterFixed.o \
Logger.o \
Main.o \
Menu.o \
OLEDFourBit.o \
OneWire.o \
OneWireTempSensor.o \
PiLink.o \
Random.o \
RotaryEncoder.o \
Sensor.o \
SettingsManager.o \
Simulator.o \
SpiLcd.o \
TempControl.o \
TemperatureFormats.o \
TempSensor.o \
Ticks.o

C_DEPS +=  \
Actuator.d \
ArduinoFunctions.d \
Brewpi.d \
BrewpiStrings.d \
Buzzer.d \
DallasTemperature.d \
DeviceManager.d \
Display.d \
DisplayLcd.d \
DS2413.d \
EepromManager.d \
FilterCascaded.d \
FilterFixed.d \
Logger.d \
Main.d \
Menu.d \
OLEDFourBit.d \
OneWire.d \
OneWireTempSensor.d \
PiLink.d \
Random.d \
RotaryEncoder.d \
Sensor.d \
SettingsManager.d \
Simulator.d \
SpiLcd.d \
TempControl.d \
TemperatureFormats.d \
TempSensor.d \
Ticks.d

C_DEPS_AS_ARGS +=  \
Actuator.d \
ArduinoFunctions.d \
Brewpi.d \
BrewpiStrings.d \
Buzzer.d \
DallasTemperature.d \
DeviceManager.d \
Display.d \
DisplayLcd.d \
DS2413.d \
EepromManager.d \
FilterCascaded.d \
FilterFixed.d \
Logger.d \
Main.d \
Menu.d \
OLEDFourBit.d \
OneWire.d \
OneWireTempSensor.d \
PiLink.d \
Random.d \
RotaryEncoder.d \
Sensor.d \
SettingsManager.d \
Simulator.d \
SpiLcd.d \
TempControl.d \
TemperatureFormats.d \
TempSensor.d \
Ticks.d

OUTPUT_FILE_PATH +=$(OUTPUT_DIR)brewpi_avr.elf

OUTPUT_FILE_PATH_AS_ARGS +=$(OUTPUT_DIR)brewpi_avr.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+= 

# AVR32/GNU C Compiler






















































./%.o: .././%.cpp
	@echo Building file: $<
	@echo Invoking: AVR8/GNU C Compiler
	"$(AVRGCC)" -mcall-prologues -funsigned-char -funsigned-bitfields -DARDUINO=105 -DF_CPU=16000000L  -I"$(ARDUINO_HEADERS)" -I"$(MCU_INCLUDE)" -I"..\fallback"  -Os -fno-split-wide-types -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -g3 -Wall -mmcu=$(MCU) -c -fno-exceptions -ffreestanding -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<"
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
	@echo Invoking: AVR8/GNU Linker : 3.4.2
	"$(AVRGCC)" -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -Wl,-Map="brewpi_avr.map" -Wl,--start-group -Wl,-lm  -Wl,--end-group -Wl,--gc-sections -mmcu=$(MCU) -Wl,--relax 
	@echo Finished building target: $@
	"$(AVROBJCOPY)" -O ihex -R .eeprom -R .fuse -R .lock -R .signature  "brewpi_avr.elf" "brewpi_avr.hex"
	
	"$(AVROBJCOPY)" -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex "brewpi_avr.elf" "brewpi_avr.eep" || exit 0
	
	"$(AVROBJDUMP)" -h -S "brewpi_avr.elf" > "brewpi_avr.lss"
	
	"$(AVROBJCOPY)" -O srec -R .eeprom -R .fuse -R .lock -R .signature  "brewpi_avr.elf" "brewpi_avr.srec"
	
	"$(AVRSIZE)" "brewpi_avr.elf"

	


# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	$(RM) -rf "brewpi_avr.elf" "brewpi_avr.a" "brewpi_avr.hex" "brewpi_avr.lss" "brewpi_avr.eep" "brewpi_avr.map" "brewpi_avr.srec"
	