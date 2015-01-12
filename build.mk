INCLUDE_DIRS += $(SOURCE_PATH)/app/controller
INCLUDE_DIRS += $(SOURCE_PATH)/app/devices
INCLUDE_DIRS += $(SOURCE_PATH)/app/fallback
INCLUDE_DIRS += $(SOURCE_PATH)/platform/wiring
INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark

CSRC += $(call target_files,app/,*.c)
CPPSRC += $(call target_files,app/,*.cpp)

CSRC += $(call target_files,platform/wiring/,*.c)
CPPSRC += $(call target_files,platform/wiring/,*.cpp)

CSRC += $(call target_files,platform/spark/,*.c)
CPPSRC += $(call target_files,platform/spark/,*.cpp)
