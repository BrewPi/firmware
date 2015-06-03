here_files = $(patsubst $(SOURCE_PATH)/%,%,$(wildcard $(SOURCE_PATH)/$1/$2))

INCLUDE_DIRS += $(SOURCE_PATH)/app/controller
INCLUDE_DIRS += $(SOURCE_PATH)/app/controller/Display
INCLUDE_DIRS += $(SOURCE_PATH)/app/controller/Filter
INCLUDE_DIRS += $(SOURCE_PATH)/app/devices
INCLUDE_DIRS += $(SOURCE_PATH)/app/devices/Actuator
INCLUDE_DIRS += $(SOURCE_PATH)/app/devices/OneWire
INCLUDE_DIRS += $(SOURCE_PATH)/app/devices/OneWireSwitch
INCLUDE_DIRS += $(SOURCE_PATH)/app/devices/TempSensor
INCLUDE_DIRS += $(SOURCE_PATH)/app/devices/Display
INCLUDE_DIRS += $(SOURCE_PATH)/app/fallback
INCLUDE_DIRS += $(SOURCE_PATH)/platform/wiring
INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark
INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark/modules
INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark/modules/Adafruit_ILI9341
INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark/modules/BrewPiTouch
INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark/modules/Display
INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark/modules/EEPROM
INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark/modules/eGUI_screens
INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark/modules/OneWire
# INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark/modules/ScrollBox
INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark/modules/Ticks
INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark/modules/UI

CSRC += $(call target_files,app/controller,*.c)
CPPSRC += $(call target_files,app/controller,*.cpp)

CSRC += $(call target_files,app/devices,*.c)
CPPSRC += $(call target_files,app/devices,*.cpp)

CSRC += $(call target_files,platform/wiring/,*.c)
CPPSRC += $(call target_files,platform/wiring/,*.cpp)

CSRC += $(call target_files,platform/spark/modules,*.c)
CPPSRC += $(call target_files,platform/spark/modules,*.cpp)

SRC_EGUI = $(SOURCE_PATH)/platform/spark/modules/eGUI
include $(SRC_EGUI)/egui.mk

$(info source path $(SOURCE_PATH))
LIBS_DIR = $(SOURCE_PATH)/platform/spark/libs
include $(LIBS_DIR)/libs.mk

CFLAGS += -fdata-sections

GIT_VERSION = $(shell cd $(SOURCE_PATH); git describe --long)
$(info using $(GIT_VERSION) as build name)
CFLAGS += -DBUILD_NAME="$(GIT_VERSION)"