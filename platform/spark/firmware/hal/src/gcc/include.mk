
ifneq (,$(findstring hal,$(MAKE_DEPENDENCIES)))

LDFLAGS += -static


# additional libraries required by gcc build
ifeq ($(MAKE_OS),WINDOWS)
LIBS += boost_system ws2_32
else
LIBS += boost_system
endif
LIBS += boost_program_options boost_random

LIB_DIRS += $(BOOST_ROOT)/stage/lib

# gcc HAL is different for test driver and test subject
ifeq "$(SPARK_TEST_DRIVER)" "1"
HAL_TEST_FLAVOR+=-driver
endif



endif



