include ../build/platform-id.mk

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

# add auto-generated protobuf includes
INCLUDE_DIRS += $(SOURCE_PATH)/app/cbox/proto/cpp
CSRC += $(call here_files,app/cbox/proto/cpp,*.c)

# add nanopb dependencies
include $(SOURCE_PATH)/platform/spark/firmware/nanopb/import.mk

ifeq ($(PLATFORM_ID),6)
MODULAR?=y
endif
ifeq ($(PLATFORM_ID),8)
MODULAR?=y
endif
ifeq ($(PLATFORM_ID),10)
MODULAR?=y
endif

ifneq ($(MODULAR),y)
# for a non-modular build, exclude the duplicate nanopb functions, because they will be available from Particle's library
# for a modular build, the file below has copies of the functions that particle discards
CEXCLUDES += app/cbox/nanopb_not_in_particle.c
endif


# enable coverage for gcc builds
ifeq ($(PLATFORM_ID),3)
CFLAGS += -g -O0 -fno-inline
CFLAGS += --coverage
LDFLAGS += -Wl,--verbose --coverage
endif

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
#INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark/modules/Ticks
#INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark/modules/UI
#INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark/modules/ValvesController

INCLUDE_DIRS += $(SOURCE_PATH)/controlbox/src/lib

INCLUDE_DIRS += $(BOOST_ROOT)

CSRC += $(call here_files,app/cbox,*.c)
CPPSRC += $(call here_files,app/cbox,*.cpp)

CPPSRC += $(call here_files,app/cbox/blox,*.cpp)

#CSRC += $(call target_files,app/devices,*.c)
#CPPSRC += $(call target_files,app/devices,*.cpp)

#CSRC += $(call target_files,platform/wiring/,*.c)
#CPPSRC += $(call target_files,platform/wiring/,*.cpp)

CSRC += $(call here_files,platform/spark/modules,*.c)
CPPSRC += $(call here_files,platform/spark/modules,*.cpp)

CPPSRC += $(call here_files,platform/spark/modules/OneWire,*.cpp)

CPPSRC += $(call here_files,platform/spark/modules/EEPROM,*.cpp)

# hardware specific includes
INCLUDE_DIRS += $(SOURCE_PATH)/app/cbox/spark
CPPSRC += $(call here_files,app/cbox/spark,*.cpp)


SRC_EGUI = $(SOURCE_PATH)/platform/spark/modules/eGUI
#include $(SRC_EGUI)/egui.mk

LIBS_DIR = $(SOURCE_PATH)/platform/spark/libs
include $(LIBS_DIR)/libs.mk

INCLUDE_DIRS += $(LIBS_DIR)/mdns/firmware
CPPSRC += $(call here_files,platform/spark/libs/mdns/firmware,*.cpp)

ifeq ("$(CBOX_DEBUG)","y")
CFLAGS += -DCBOX_DEBUG=1
else
CFLAGS += -DCBOX_DEBUG=0
endif

# the following warnings can help find opportunities for impromevent in virtual functions
# they are disabled in the default build, because the dependencies (particle firmware, flashee) have many violations 

# Warn when virtual functions are overriden without override/override final specifier (requires gcc 5.1)
# CPPFLAGS += -Wsuggest-override
# Warn when functions and classes can be marked final
# CPPFLAGS += -Wsuggest-final-types
# CPPFLAGS += -Wsuggest-final-methods

CSRC := $(filter-out $(CEXCLUDES),$(CSRC))
CPPSRC := $(filter-out $(CPPEXCLUDES),$(CPPSRC)) 

GIT_VERSION = $(shell cd $(SOURCE_PATH); git describe --long)
$(info using $(GIT_VERSION) as build name)
CFLAGS += -DBUILD_NAME="$(GIT_VERSION)"

COMPILER_VERSION = $(shell $(CC) --version) 
$(info using compiler: $(COMPILER_VERSION)) 
 