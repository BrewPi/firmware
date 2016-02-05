here_files = $(patsubst $(SOURCE_PATH)/%,%,$(wildcard $(SOURCE_PATH)/$1/$2))


INCLUDE_DIRS += $(SOURCE_PATH)/app/cbox
INCLUDE_DIRS += $(SOURCE_PATH)/lib/inc
INCLUDE_DIRS += $(SOURCE_PATH)/platform/wiring
#INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark
INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark/modules
#INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark/modules/Adafruit_ILI9341
#INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark/modules/Adafruit_mfGFX
#INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark/modules/BrewPiTouch
#INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark/modules/DS2408
#INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark/modules/DS2413
#INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark/modules/Display
#INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark/modules/EEPROM
#INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark/modules/LowPassFilter
INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark/modules/OneWire
#INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark/modules/OneWireSwitch
#INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark/modules/ScrollBox
INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark/modules/Ticks
#INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark/modules/UI
#INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark/modules/ValvesController

INCLUDE_DIRS += $(SOURCE_PATH)/../controlbox/lib

INCLUDE_DIRS += $(BOOST_ROOT)

CSRC += $(call target_files,../cbox,*.c)
CPPSRC += $(call target_files,../cbox,*.cpp)


#CSRC += $(call target_files,app/devices,*.c)
#CPPSRC += $(call target_files,app/devices,*.cpp)

#CSRC += $(call target_files,platform/wiring/,*.c)
#ßCPPSRC += $(call target_files,platform/wiring/,*.cpp)

SRC_EGUI = $(SOURCE_PATH)/platform/spark/modules/eGUI
#include $(SRC_EGUI)/egui.mk

LIBS_DIR = $(SOURCE_PATH)/platform/spark/libs
include $(LIBS_DIR)/libs.mk

GIT_VERSION = $(shell cd $(SOURCE_PATH); git describe --long)
$(info using $(GIT_VERSION) as build name)
CFLAGS += -DBUILD_NAME="$(GIT_VERSION)"