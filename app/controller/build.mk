# here_files is a non-recursive file search. target_files is recursive.
here_files = $(patsubst $(SOURCE_PATH)/%,%,$(wildcard $(SOURCE_PATH)/$1/$2))

INCLUDE_DIRS += $(SOURCE_PATH)/app/controller
INCLUDE_DIRS += $(SOURCE_PATH)/app/controller/Display
INCLUDE_DIRS += $(SOURCE_PATH)/app/controller/Filter
INCLUDE_DIRS += $(SOURCE_PATH)/app/controller/esj
INCLUDE_DIRS += $(SOURCE_PATH)/app/controller/mixins
INCLUDE_DIRS += $(SOURCE_PATH)/lib/inc
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
INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark/modules/Ticks
INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark/modules/UI
INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark/modules/Buzzer

CSRC += $(call target_files,app/controller,*.c)
CPPSRC += $(call target_files,app/controller,*.cpp)

CEXCLUDES += $(call target_files,app/controller/test,*.c)
CPPEXCLUDES += $(call target_files,app/controller/test,*.cpp) 

CSRC += $(call target_files,lib/src,*.c)
CPPSRC += $(call target_files,lib/src,*.cpp)

CSRC += $(call target_files,platform/wiring/,*.c)
CPPSRC += $(call target_files,platform/wiring/,*.cpp)

CSRC += $(call target_files,platform/spark/modules,*.c)
CPPSRC += $(call target_files,platform/spark/modules,*.cpp)

ifeq ($(BOOST_ROOT),)
$(error BOOST_ROOT not set. Download boost and add BOOST_ROOT to your environment variables.)
endif
CFLAGS += -I$(BOOST_ROOT)

ifeq ("$(PLATFORM_ID)","0")
# disable freertos for the core, until we have more free space.
FREERTOS=0
CFLAGS += -DFREERTOS=0
# disable big logo on the core, until we have more free space
CFLAGS += -DBREWPI_BIG_LOGO=0
endif

SRC_EGUI = $(SOURCE_PATH)/platform/spark/modules/eGUI
include $(SRC_EGUI)/egui.mk

$(info source path $(SOURCE_PATH))
LIBS_DIR = $(SOURCE_PATH)/platform/spark/libs
include $(LIBS_DIR)/libs.mk

CFLAGS += -fdata-sections

GIT_VERSION = $(shell cd $(SOURCE_PATH); git describe --long)
$(info using $(GIT_VERSION) as build name)
CFLAGS += -DBUILD_NAME="$(GIT_VERSION)"

CFLAGS += -Wall 
CPPFLAGS += -Woverloaded-virtual

CSRC := $(filter-out $(CEXCLUDES),$(CSRC))
CPPSRC := $(filter-out $(CPPEXCLUDES),$(CPPSRC)) 

