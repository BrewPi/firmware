INCLUDE_DIRS += $(SOURCE_PATH)/app/egui_demo
INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark
INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark/modules/BrewPiTouch
INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark/modules/LowPassFilter

CSRC += $(call target_files,app/egui_demo,*.c)
CPPSRC += $(call target_files,app/egui_demo,*.cpp)

CSRC += $(call target_files,platform/spark/modules/eGUI,*.c)
CPPSRC += $(call target_files,platform/spark/modules/eGUI,*.cpp)

CPPSRC += $(call target_files,platform/spark/modules/BrewPiTouch,*.cpp)
CPPSRC += $(call target_files,platform/spark/modules/LowPassFilter,*.cpp)

SRC_EGUI = $(SOURCE_PATH)/platform/spark/modules/eGUI
include $(SRC_EGUI)/egui.mk

LIBS_DIR = $(SOURCE_PATH)/platform/spark/libs
include $(LIBS_DIR)/libs.mk