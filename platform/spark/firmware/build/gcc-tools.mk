
GCC_PREFIX=

include $(COMMON_BUILD)/common-tools.mk
include $(COMMON_BUILD)/os.mk

#
# default flags for targeting ARM
#

GCC_OPTIMIZE=3
ifeq ($(DEBUG_BUILD),y)
     GCC_OPTIMIZE=0
endif

# C compiler flags
CFLAGS +=  -g3 -O$(GCC_OPTIMIZE) -gdwarf-2
ifneq ("$(MAKE_OS)", "WINDOWS")
CFLAGS += -Wno-unused-local-typedefs -Wno-return-type-c-linkage 
CPPFLAGS += -Wno-unused-private-field
else
# boost now uses i686 as the target architecture for 32-bit targets
# need to specify this to avoid duplicate sections of different sizes. 
# http://stackoverflow.com/a/29736626/326480
CFLAGS += -march=i686 -gc-sections
endif

ASFLAGS +=  -g3


ifeq ("$(MAKE_OS)", "LINUX")
LDFLAGS +=  -pthread
endif

ifneq ($(MAKE_OS),OSX)
LDFLAGS += -Xlinker --gc-sections
endif





