# Include makefile for setting up the include folders for libraries
# LIBS_DIR is the directory containing this file

INCLUDE_DIRS += $(LIBS_DIR)/flashee/firmware
CPPSRC += $(call here_files,platform/spark/libs/flashee/firmware,*.cpp)


ifeq ("$(PLATFORM_ID)","0")
ifneq ("$(FREERTOS)","0")
INCLUDE_DIRS += $(LIBS_DIR)/freertos4core/firmware
CPPSRC += $(call here_files,platform/spark/libs/freertos4core/firmware,*.cpp)
CSRC += $(call here_files,platform/spark/libs/freertos4core/firmware,*.c)
LDFLAGS += -u vTaskSwitchContext
endif
endif

