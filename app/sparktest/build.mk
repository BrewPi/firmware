here_files = $(patsubst $(SOURCE_PATH)/%,%,$(wildcard $(SOURCE_PATH)/$1/$2))

INCLUDE_DIRS += $(SOURCE_PATH)/app/sparktest
INCLUDE_DIRS += $(SOURCE_PATH)/app/devices
INCLUDE_DIRS += $(SOURCE_PATH)/app/fallback
INCLUDE_DIRS += $(SOURCE_PATH)/app/devices
INCLUDE_DIRS += $(SOURCE_PATH)/app/devices/Actuator
INCLUDE_DIRS += $(SOURCE_PATH)/app/devices/OneWire
INCLUDE_DIRS += $(SOURCE_PATH)/app/devices/OneWireSwitch
INCLUDE_DIRS += $(SOURCE_PATH)/app/devices/TempSensor
INCLUDE_DIRS += $(SOURCE_PATH)/app/devices/ValvesController
INCLUDE_DIRS += $(SOURCE_PATH)/platform/wiring
INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark
INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark/modules
INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark/modules/Adafruit_ILI9341
INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark/modules/Adafruit_mfGFX
INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark/modules/BrewPiTouch
INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark/modules/OneWire
INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark/modules/ScrollBox
INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark/modules/Ticks


CSRC += $(call target_files,app/sparktest,*.c)
CPPSRC += $(call target_files,app/sparktest,*.cpp)

CPPSRC += $(call target_files,app/devices/ValvesController,*.cpp)
CPPSRC += $(call target_files,app/devices/OneWire,*.cpp)

CPPSRC += $(call target_files,platform/spark/modules/Adafruit_ILI9341,*.cpp)
CPPSRC += $(call target_files,platform/spark/modules/Adafruit_mfGFX,*.cpp)
CPPSRC += $(call target_files,platform/spark/modules/OneWire,*.cpp)
CPPSRC += $(call target_files,platform/spark/modules/ScrollBox,*.cpp)
CPPSRC += $(call target_files,platform/spark/modules/BrewPiTouch,*.cpp)


#SRC_EGUI = $(SOURCE_PATH)/platform/spark/modules/eGUI
#include $(SRC_EGUI)/egui.mk

#CSRC += $(call target_files,platform/wiring/,*.c)
#CPPSRC += $(call target_files,platform/wiring/,*.cpp)

#CSRC += $(call target_files,platform/spark/,*.c)
#CPPSRC += $(call target_files,platform/spark/,*.cpp)
GIT_VERSION = $(shell cd $(SOURCE_PATH); git describe --long)
$(info using $(GIT_VERSION) as build name)
CFLAGS += -DBUILD_NAME="$(GIT_VERSION)"
