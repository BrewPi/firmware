here_files = $(patsubst $(SOURCE_PATH)/%,%,$(wildcard $(SOURCE_PATH)/$1/$2))


# add all objects the user can create
INCLUDE_DIRS += $(SOURCE_PATH)/app/cbox/blox

# add all lib source files
INCLUDE_DIRS += $(SOURCE_PATH)/lib/inc
CSRC += $(call target_files,lib/src,*.c)
CPPSRC += $(call target_files,lib/src,*.cpp)

# add all cbox lib source files
INCLUDE_DIRS += $(SOURCE_PATH)/controlbox/src/lib
CPPSRC += $(call here_files,controlbox/src/lib/,*.cpp)
CFLAGS += -DCONTROLBOX_STATIC=1 # use static controlbox implementation

# add auto-generated protobuf includes
INCLUDE_DIRS += $(SOURCE_PATH)/app/cbox/proto/cpp
CSRC += $(call here_files,app/cbox/proto/cpp,*.c)

# add nanopb dependencies
include $(SOURCE_PATH)/platform/spark/firmware/nanopb/import.mk

# Mixins
CPPSRC += app/cbox/ControllerMixins.cpp

# App
INCLUDE_DIRS += $(SOURCE_PATH)/app
INCLUDE_DIRS += $(SOURCE_PATH)/app/fallback
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

INCLUDE_DIRS += $(BOOST_ROOT)

CSRC += $(call here_files,app/cbox,*.c)
CPPSRC += $(call here_files,app/cbox,*.cpp)

#CSRC += $(call target_files,app/devices,*.c)
#CPPSRC += $(call target_files,app/devices,*.cpp)

CSRC += $(call target_files,platform/wiring/,*.c)
CPPSRC += $(call target_files,platform/wiring/,*.cpp)

CSRC += $(call here_files,platform/spark/modules,*.c)
CPPSRC += $(call here_files,platform/spark/modules,*.cpp)

CPPSRC += $(call here_files,platform/spark/modules/OneWire,*.cpp)

CPPSRC += $(call here_files,platform/spark/modules/EEPROM,*.cpp)


SRC_EGUI = $(SOURCE_PATH)/platform/spark/modules/eGUI
#include $(SRC_EGUI)/egui.mk

LIBS_DIR = $(SOURCE_PATH)/platform/spark/libs
include $(LIBS_DIR)/libs.mk

INCLUDE_DIRS += $(LIBS_DIR)/mdns/firmware
CPPSRC += $(call here_files,platform/spark/libs/mdns/firmware,*.cpp)

ifeq ("$(CBOX_DEBUG)","y")
CFLAGS += -DCBOX_DEBUG=1
endif

GIT_VERSION = $(shell cd $(SOURCE_PATH); git describe --long)
$(info using $(GIT_VERSION) as build name)
CFLAGS += -DBUILD_NAME="$(GIT_VERSION)"

COMPILER_VERSION = $(shell $(CC) --version) 
$(info using compiler: $(COMPILER_VERSION)) 
 