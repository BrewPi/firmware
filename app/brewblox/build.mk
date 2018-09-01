include ../build/platform-id.mk

here_files = $(patsubst $(SOURCE_PATH)/%,%,$(wildcard $(SOURCE_PATH)/$1/$2))

# add all objects the user can create
INCLUDE_DIRS += $(SOURCE_PATH)/app/cbox/blox

# add all lib source files
INCLUDE_DIRS += $(SOURCE_PATH)/lib/inc
CSRC += $(call target_files,lib/src,*.c)
CPPSRC += $(call target_files,lib/src,*.cpp)

# add all cbox lib source files
INCLUDE_DIRS += $(SOURCE_PATH)/controlbox/src/
CPPSRC += $(call here_files,controlbox/src/cbox/,*.cpp)

# add auto-generated protobuf includes
INCLUDE_DIRS += $(SOURCE_PATH)/app/brewblox/proto/cpp
CSRC += $(call here_files,app/brewblox/proto/cpp,*.c)

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
EXTRA_CFLAGS += -g -O0 -fno-inline
EXTRA_CFLAGS += --coverage
LDFLAGS += -Wl,--verbose --coverage
endif

# App
INCLUDE_DIRS += $(SOURCE_PATH)/app
INCLUDE_DIRS += $(SOURCE_PATH)/app/fallback
INCLUDE_DIRS += $(SOURCE_PATH)/app/brewblox
INCLUDE_DIRS += $(SOURCE_PATH)/controlbox/src
INCLUDE_DIRS += $(SOURCE_PATH)/controlbox/src

# include boost
ifeq ($(BOOST_ROOT),)
$(error BOOST_ROOT not set. Download boost and add BOOST_ROOT to your environment variables.)
endif
EXTRA_CFLAGS += -isystem $(BOOST_ROOT)

CSRC += $(call here_files,app/brewblox,*.c)
CPPSRC += $(call here_files,app/brewblox,*.cpp)
CPPSRC += $(call here_files,app/brewblox/blox,*.cpp)

CSRC += $(call here_files,platform/wiring/,*.c)
CPPSRC += $(call here_files,platform/wiring/,*.cpp)

INCLUDE_DIRS += $(SOURCE_PATH)/platform
CSRC += $(call here_files,platform/spark/modules/Platform,*.c)
CPPSRC += $(call here_files,platform/spark/modules/Platform,*.cpp)

CSRC += $(call here_files,platform/spark/modules/Board,*.c)
CPPSRC += $(call here_files,platform/spark/modules/Board,*.cpp)

CPPSRC += $(call here_files,platform/spark/modules/OneWire,*.cpp)
CPPSRC += $(call here_files,platform/spark/modules/EEPROM,*.cpp)

# hardware specific includes
INCLUDE_DIRS += $(SOURCE_PATH)/app/brewblox/spark
CPPSRC += $(call here_files,app/brewblox/spark,*.cpp)


SRC_EGUI = $(SOURCE_PATH)/platform/spark/modules/eGUI
#include $(SRC_EGUI)/egui.mk

LIBS_DIR = $(SOURCE_PATH)/platform/spark/libs
include $(LIBS_DIR)/libs.mk

INCLUDE_DIRS += $(LIBS_DIR)/mdns/firmware
CPPSRC += $(call here_files,platform/spark/libs/mdns/firmware,*.cpp)

ifeq ("$(CBOX_DEBUG)","y")
EXTRA_CFLAGS += -DCBOX_DEBUG=1
else
EXTRA_CFLAGS += -DCBOX_DEBUG=0
endif

# the following warnings can help find opportunities for impromevent in virtual functions
# they are disabled in the default build, because the dependencies (particle firmware, flashee) have many violations 

# Warn when virtual functions are overriden without override/override final specifier (requires gcc 5.1)
CPPFLAGS += -Wsuggest-override
# Warn when functions and classes can be marked final
CPPFLAGS += -Wsuggest-final-types
CPPFLAGS += -Wsuggest-final-methods

CSRC := $(filter-out $(CEXCLUDES),$(CSRC))
CPPSRC := $(filter-out $(CPPEXCLUDES),$(CPPSRC)) 

GIT_VERSION = $(shell cd $(SOURCE_PATH); git describe --long)
$(info using $(GIT_VERSION) as build name)
EXTRA_CFLAGS += -DBUILD_NAME="$(GIT_VERSION)"

COMPILER_VERSION = $(shell $(CC) --version) 
$(info using compiler: $(COMPILER_VERSION)) 
 
# recompile proto files
$(info compiling proto files) 
PROTO_OUTPUT = $(shell bash $(SOURCE_PATH)/app/brewblox/proto/generate_proto_cpp.sh)