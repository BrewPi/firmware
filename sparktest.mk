INCLUDE_DIRS += $(SOURCE_PATH)/app/sparktest
INCLUDE_DIRS += $(SOURCE_PATH)/app/devices
INCLUDE_DIRS += $(SOURCE_PATH)/app/fallback
INCLUDE_DIRS += $(SOURCE_PATH)/app/devices
INCLUDE_DIRS += $(SOURCE_PATH)/platform/wiring
INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark
INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark/devices/Adafruit_ILI9341
INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark/devices/Adafruit_mfGFX
INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark/devices/BrewPiTouch
INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark/devices/DS2408
INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark/devices/DS2413
INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark/devices/DS2482
INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark/devices/Display
INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark/devices/EEPROM
INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark/devices/LowPassFilter
INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark/devices/OneWire
INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark/devices/OneWireSwitch
INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark/devices/ScrollBox
INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark/devices/Ticks
INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark/devices/ValvesController


CSRC += $(call target_files,app/sparktest,*.c)
CPPSRC += $(call target_files,app/sparktest,*.cpp)

CSRC += $(call target_files,platform/spark/devices/,*.c)
CPPSRC += $(call target_files,platform/spark/devices/,*.cpp)

#CSRC += $(call target_files,platform/wiring/,*.c)
#CPPSRC += $(call target_files,platform/wiring/,*.cpp)

#CSRC += $(call target_files,platform/spark/,*.c)
#CPPSRC += $(call target_files,platform/spark/,*.cpp)
