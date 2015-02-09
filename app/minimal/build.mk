INCLUDE_DIRS += $(SOURCE_PATH)/platform/wiring
INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark

CSRC += $(call target_files,app/minimal,*.c)
CPPSRC += $(call target_files,app/minimal,*.cpp)