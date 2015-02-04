# Include makefile for setting up the include folders for libraries
# LIBS_DIR is the directory containing this file

INCLUDE_DIRS += $(LIBS_DIR)/flashee/firmware

CPPSRC += $(call here_files,platform/spark/libs/flashee/firmware,*.cpp)

