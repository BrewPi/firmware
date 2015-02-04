# Include makefile for setting up the include folders for libraries
# LIBS_DIR is the directory containing this file

INCLUDE_DIRS += $(LIBS_DIR)/flashee/firmware

CSRC += $(call here_files,$(LIBS_DIR)/firmware,*.c)
CPPSRC += $(call here_files,$(LIBS_DIR)/firmware,*.cpp)

