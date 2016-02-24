## -*- Makefile -*-

CCC = gcc
CXX = g++
LD = g++
CFLAGS = -g
CCFLAGS = $(CFLAGS)
CXXFLAGS = $(CFLAGS) $(CPPFLAGS)
RM = rm -f
RMDIR = rm -f -r
MKDIR = mkdir -p

PROJECT_ROOT=.
LIB_ROOT=$(PROJECT_ROOT)/src/lib
COMMON_BUILD=$(PROJECT_ROOT)/build

TARGETDIR=$(COMMON_BUILD)/target/final
BUILD_PATH=$(COMMON_BUILD)/target/obj
TARGET=controlbox

# enumerates files in the filesystem and returns their path relative to the project root
# $1 the directory relative to the project root
# $2 the pattern to match, e.g. *.cpp
rwildcard = $(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))
target_files += $(patsubst $(PROJECT_ROOT)/%,%,$(call rwildcard,$(PROJECT_ROOT)/$1,$2))

CPPSRC += $(call target_files,src/lib/,*.cpp)
CPPSRC += $(call target_files,app/test/,*.cpp)

include $(call rwildcard,$(PROJECT_ROOT)/,include.mk)

# add include dirs to the compiler include path
CFLAGS += $(patsubst %,-I%,$(INCLUDE_DIRS)) -I.
CFLAGS += -ffunction-sections -fdata-sections -Wall

# Generate dependency files automatically.
CFLAGS += -MD -MP -MF $@.d
# enable profiling and code coverage
CFLGAS += -fprofile-arcs -ftest-coverage

CPPFLAGS += -std=gnu++11
CPPFLAGS += -DCATCH_CONFIG_SFINAE

# Collect all object and dep files
ALLOBJ += $(addprefix $(BUILD_PATH)/, $(CPPSRC:.cpp=.o))
ALLDEPS += $(addprefix $(BUILD_PATH)/, $(CPPSRC:.cpp=.o.d))

ifeq ("$(BOOST_ROOT)","")
$(error "BOOST_ROOT should point to the base folder for boost")
endif

INCLUDE_DIRS += $(BOOST_ROOT)

all: test

test: $(TARGETDIR)/$(TARGET)

$(TARGETDIR)/$(TARGET) : $(TARGETDIR) $(BUILD_PATH) $(ALLOBJ)
	@echo Building target: $@
	@echo Invoking: GCC C++ Linker
	$(MKDIR) $(dir $@)
	$(LD) $(CFLAGS) $(ALLOBJ) --output $@ $(LDFLAGS)
	@echo

$(TARGETDIR):
	@echo Creating target dir
	$(MKDIR) $(TARGETDIR)

$(BUILD_PATH):
	@echo Creating build path
	$(MKDIR) $(BUILD_PATH)


# Tool invocations

# CPP compiler to build .o from .cpp in $(BUILD_DIR)
# Note: Calls standard $(CC) - gcc will invoke g++ as appropriate
$(BUILD_PATH)/%.o : $(PROJECT_ROOT)/%.cpp
	@echo Building file: $<
	@echo Invoking: GCC CPP Compiler
	$(MKDIR) $(dir $@)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<
	@echo

# Other Targets
clean:
	$(RM) $(ALLOBJ) $(ALLDEPS) $(TARGETDIR)/$(TARGET)
	$(RMDIR) $(TARGETDIR)
	$(RMDIR) $(BUILD_PATH)
	@echo

.PHONY: all clean test
.SECONDARY:

# Include auto generated dependency files
-include $(ALLDEPS)



