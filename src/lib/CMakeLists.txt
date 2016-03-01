

set(srcs Commands.cpp Comms.cpp DataStream.cpp GenericContainer.cpp Integration.cpp Memops.cpp SystemProfile.cpp Values.cpp ValuesEeprom.cpp) 

FUNCTION(PREPEND var prefix)
   SET(listVar "")
   FOREACH(f ${ARGN})
      LIST(APPEND listVar "${prefix}/${f}")
   ENDFOREACH(f)
   SET(${var} "${listVar}" PARENT_SCOPE)
ENDFUNCTION(PREPEND)

PREPEND(cbox_lib_src ${CMAKE_CURRENT_SOURCE_DIR} ${srcs})

set(cbox_lib_inc ${CMAKE_CURRENT_SOURCE_DIR})

if(DEFINED ENV{BOOST_ROOT})
	message("Using BOOST_ROOT=$ENV{BOOST_ROOT}")
	set(cbox_lib_inc ${cbox_lib_inc} $ENV{BOOST_ROOT})
else()
	message(SEND_ERROR "BOOST_ROOT environment varaible not defined.")
endif()

set_property(GLOBAL PROPERTY cbox_lib_src ${cbox_lib_src})
set_property(GLOBAL PROPERTY cbox_lib_inc ${cbox_lib_inc})
set_property(GLOBAL PROPERTY cbox_lib_dir ${CMAKE_CURRENT_SOURCE_DIR})
 