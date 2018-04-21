include ../build/platform-id.mk

INCLUDE_DIRS += $(SOURCE_PATH)/app
INCLUDE_DIRS += $(SOURCE_PATH)/app/fallback
INCLUDE_DIRS += $(SOURCE_PATH)/app/rs485-listener
INCLUDE_DIRS += $(SOURCE_PATH)/platform/wiring
INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark
INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark/modules
INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark/modules/OneWire
INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark/modules/EEPROM
INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark/modules/Ticks
INCLUDE_DIRS += $(SOURCE_PATH)/lib/inc

here_files = $(patsubst $(SOURCE_PATH)/%,%,$(wildcard $(SOURCE_PATH)/$1/$2))

ifeq ($(PLATFORM_ID),6)
MODULAR?=y
endif
ifeq ($(PLATFORM_ID),8)
MODULAR?=y
endif
ifeq ($(PLATFORM_ID),10)
MODULAR?=y
endif

CPPSRC += $(call here_files,app/rs485-listener,*.cpp)
CSRC += $(call here_files,platform/spark/modules,*.c)
CPPSRC += $(call here_files,platform/spark/modules,*.cpp)
CPPSRC += $(call here_files,platform/spark/modules/OneWire,*.cpp)
CPPSRC += $(call here_files,platform/spark/modules/EEPROM,*.cpp)


LIBS_DIR = $(SOURCE_PATH)/platform/spark/libs
include $(LIBS_DIR)/libs.mk

CSRC := $(filter-out $(CEXCLUDES),$(CSRC))
CPPSRC := $(filter-out $(CPPEXCLUDES),$(CPPSRC)) 

GIT_VERSION = $(shell cd $(SOURCE_PATH); git describe --long)
$(info using $(GIT_VERSION) as build name)
CFLAGS += -DBUILD_NAME="$(GIT_VERSION)"

COMPILER_VERSION = $(shell $(CC) --version) 
$(info using compiler: $(COMPILER_VERSION)) 
 