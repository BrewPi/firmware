here_files = $(patsubst $(SOURCE_PATH)/%,%,$(wildcard $(SOURCE_PATH)/$1/$2))


INCLUDE_DIRS += $(SOURCE_PATH)/app/cbox
INCLUDE_DIRS += $(SOURCE_PATH)/platform/wiring
INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark
INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark/modules
#INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark/modules/BrewPiTouch
#INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark/modules/DS2413
INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark/modules/EEPROM
#INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark/modules/LowPassFilter
INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark/modules/OneWire
#INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark/modules/OneWireSwitch
#INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark/modules/ScrollBox
INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark/modules/Ticks
#INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark/modules/UI
#INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark/modules/ValvesController

INCLUDE_DIRS += $(SOURCE_PATH)/controlbox/src/lib
INCLUDE_DIRS += $(SOURCE_PATH)/lib/inc
INCLUDE_DIRS += $(SOURCE_PATH)/app/fallback
INCLUDE_DIRS += $(SOURCE_PATH)/app
INCLUDE_DIRS += $(SOURCE_PATH)/app/cbox/proto/cpp

INCLUDE_DIRS += $(BOOST_ROOT)

CSRC += $(call target_files,app/cbox,*.c)
CPPSRC += $(call target_files,app/cbox,*.cpp)

CPPSRC += $(call here_files,controlbox/src/lib,*.cpp)

CSRC += $(call target_files,lib/src,*.c)
CPPSRC += $(call target_files,lib/src,*.cpp)

#CSRC += $(call target_files,app/devices,*.c)
#CPPSRC += $(call target_files,app/devices,*.cpp)

CSRC += $(call target_files,platform/wiring/,*.c)
CPPSRC += $(call target_files,platform/wiring/,*.cpp)

CSRC += $(call here_files,platform/spark/modules,*.c)
CPPSRC += $(call here_files,platform/spark/modules,*.cpp)

CPPSRC += $(call here_files,platform/spark/modules/OneWire,*.cpp)


CPPSRC += $(call here_files,platform/spark/modules/EEPROM,*.cpp)

CSRC += $(call here_files,app/cbox/proto/cpp,*.c)


SRC_EGUI = $(SOURCE_PATH)/platform/spark/modules/eGUI
#include $(SRC_EGUI)/egui.mk

LIBS_DIR = $(SOURCE_PATH)/platform/spark/libs
include $(LIBS_DIR)/libs.mk

INCLUDE_DIRS += $(LIBS_DIR)/mdns/firmware
CPPSRC += $(call here_files,platform/spark/libs/mdns/firmware,*.cpp)

GIT_VERSION = $(shell cd $(SOURCE_PATH); git describe --long)
$(info using $(GIT_VERSION) as build name)
CFLAGS += -DBUILD_NAME="$(GIT_VERSION)"

COMPILER_VERSION = $(shell $(CC) --version) 
$(info using compiler: $(COMPILER_VERSION)) 
 