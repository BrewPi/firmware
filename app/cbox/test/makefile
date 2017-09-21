## -*- Makefile -*-

CCC = gcc
CXX = g++
LD = g++
CFLAGS = -g
CCFLAGS = $(CFLAGS)
CXXFLAGS = $(CFLAGS)
RM = rm -f
RMDIR = rm -f -r
MKDIR = mkdir -p

# location of this folder relative to the root
SRC_PATH=app/cbox/test
TARGETDIR=obj/
TARGET=runner

mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
current_dir := $(notdir $(patsubst %/,%,$(dir $(mkfile_path))))
SOURCE_PATH = $(abspath $(current_dir)/../../../..)

BUILD_PATH=$(TARGETDIR)test/
# Define the target directories. Nest 2 levels deep since we also include
# sources from libraries via ../core-common-lib

# Recursive wildcard function
rwildcard = $(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

# enumerates files in the filesystem and returns their path relative to the project root
# $1 the directory relative to the project root
# $2 the pattern to match, e.g. *.cpp
target_files = $(patsubst $(SOURCE_PATH)%,%,$(call rwildcard,$(SOURCE_PATH)$1,$2))

# here_files is a non-recursive file search. target_files is recursive.
here_files = $(patsubst $(SOURCE_PATH)/%,%,$(wildcard $(SOURCE_PATH)/$1/$2))

# add all tests
CPPSRC += $(call here_files,app/cbox/test,*.cpp)

# add catch test runner and dependencies
INCLUDE_DIRS += $(SOURCE_PATH)/platform/catch_test/inc
CSRC += $(call here_files,platform/catch_test/src,*.c)
CPPSRC += $(call here_files,platform/catch_test/src,*.cpp)

# add all objects the user can create
INCLUDE_DIRS += $(SOURCE_PATH)/app/cbox/blox

# add all lib source files
INCLUDE_DIRS += $(SOURCE_PATH)/lib/inc
CSRC += $(call target_files,lib/src,*.c)
CPPSRC += $(call target_files,lib/src,*.cpp)

# add all cbox lib source files
INCLUDE_DIRS += $(SOURCE_PATH)/controlbox/src/lib
CSRC += $(call target_files,/controlbox/src/lib/,*.c)
CPPSRC += $(call target_files,/controlbox/src/lib/,*.cpp)

# add auto-generated protobuf includes
INCLUDE_DIRS += $(SOURCE_PATH)/app/cbox/proto/cpp
CSRC += $(call here_files,app/cbox/proto/cpp,*.c)

# add nanopb dependencies
INCLUDE_DIRS += $(SOURCE_PATH)/nanopb
CSRC += $(call here_files,nanopb,*.c)

# Mixins
CPPSRC += app/cbox/ControllerMixins.cpp

# eeprom implementations from Spark/Particle
INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark/modules/EEPROM
INCLUDE_DIRS += $(SOURCE_PATH)/platform/spark/libs/flashee/firmware/

# include boost
ifeq ($(BOOST_ROOT),)
$(error BOOST_ROOT not set. Download boost and add BOOST_ROOT to your environment variables.)
endif
CFLAGS += -I$(BOOST_ROOT)

#INCLUDE_DIRS += $(SOURCE_PATH)app/fallback
INCLUDE_DIRS += $(SOURCE_PATH)/lib/inc
INCLUDE_DIRS += $(SOURCE_PATH)/app/cbox

CFLAGS += $(patsubst %,-I%,$(INCLUDE_DIRS)) -I.
CFLAGS += -ffunction-sections -Wall

# Flag compiler error for [-Wdeprecated-declarations]
CFLAGS += -Werror=deprecated-declarations

# Generate dependency files automatically.
CFLAGS += -MD -MP -MF $@.d
CFLAGS += -DDEBUG_BUILD
# OSX includes sys/wait.h which defines "wait"
CFLAGS += -D_SYS_WAIT_H_ -D_SYS_WAIT_H

CPPFLAGS += -std=gnu++11
CFLAGS += -pthread

# doesn't work on osx
#LDFLAGS +=  -Wl,--gc-sections 

# Collect all object and dep files
ALLOBJ += $(addprefix $(BUILD_PATH), $(CSRC:.c=.o))
ALLOBJ += $(addprefix $(BUILD_PATH), $(CPPSRC:.cpp=.o))

ALLDEPS += $(addprefix $(BUILD_PATH), $(CSRC:.c=.o.d))
ALLDEPS += $(addprefix $(BUILD_PATH), $(CPPSRC:.cpp=.o.d))

all: proto runner

proto:
	@echo Generating nanobp protobuf c files
	$(SOURCE_PATH)/app/cbox/proto/generate_proto_cpp.sh

runner: $(TARGETDIR)$(TARGET)

$(TARGETDIR)$(TARGET) : $(BUILD_PATH) $(ALLOBJ)
	@echo Building target: $@
	@echo Invoking: GCC C++ Linker
	$(MKDIR) $(dir $@)
	$(LD) $(CFLAGS) $(ALLOBJ) --output $@ $(LDFLAGS)
	@echo

$(BUILD_PATH): 
	$(MKDIR) $(BUILD_PATH)

# Tool invocations

# C compiler to build .o from .c in $(BUILD_DIR)
$(BUILD_PATH)%.o : $(SOURCE_PATH)/%.c
	@echo Building file: $<
	@echo Invoking: GCC C Compiler
	$(MKDIR) $(dir $@)
	$(CCC) $(CCFLAGS) -c -o $@ $<
	@echo

# CPP compiler to build .o from .cpp in $(BUILD_DIR)
# Note: Calls standard $(CC) - gcc will invoke g++ as appropriate
$(BUILD_PATH)%.o : $(SOURCE_PATH)/%.cpp
	@echo Building file: $<
	@echo Invoking: GCC CPP Compiler
	$(MKDIR) $(dir $@)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<
	@echo

# Other Targets
clean:	
	$(RM) $(ALLOBJ) $(ALLDEPS) $(TARGETDIR)$(TARGET)
	$(RMDIR) $(TARGETDIR)
	@echo

# print variable by invoking make print-VARIABLE as VARIABLE = the_value_of_the_variable
print-%  : ; @echo $* = $($*)

.PHONY: all clean proto runner
.SECONDARY:

# Include auto generated dependency files
-include $(ALLDEPS)



