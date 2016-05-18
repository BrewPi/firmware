
ifneq (,$(findstring hal,$(MAKE_DEPENDENCIES)))

# additional libraries required by gcc build
ifeq ($(MAKE_OS),WINDOWS)
LIBS += boost_system-mt ws2_32 mswsock
LDFLAGS += -static
flavor=-mt
else
LIBS += boost_system
LDFLAGS += -lc
flavor=
endif
LIBS += boost_program_options$(flavor) boost_random$(flavor) boost_thread$(flavor)

LIB_DIRS += $(BOOST_ROOT)/stage/lib

# gcc HAL is different for test driver and test subject
ifeq "$(SPARK_TEST_DRIVER)" "1"
HAL_TEST_FLAVOR+=-driver
endif



endif



