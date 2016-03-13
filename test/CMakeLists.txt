cmake_minimum_required (VERSION 3.4.3)
set (CMAKE_CXX_STANDARD 11)

set(test_SRC
main.cpp 
events.cpp
examplebox_tests.cpp
${cbox_examples}/shared/timems.cpp ../src/lib/BoxApi.h catch_output.h)


# retrieve the source and include paths for the library
GET_PROPERTY(cbox_lib_src GLOBAL PROPERTY cbox_lib_src)
GET_PROPERTY(cbox_lib_inc GLOBAL PROPERTY cbox_lib_inc)

# convert from a single string to a list
separate_arguments(cbox_lib_src)
separate_arguments(cbox_lib_inc)

add_executable(cbtest ${test_SRC} ${cbox_lib_src})
target_compile_definitions(cbtest PUBLIC CONTROLBOX_STATIC=0)
target_include_directories(cbtest PUBLIC ${cbox_lib_inc})

target_include_directories(cbtest PUBLIC ${cbox_examples}/shared)
target_include_directories(cbtest PUBLIC ${cbox_examples}/non-static)
target_include_directories(cbtest PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

enable_testing()
add_test(NAME unit_tests COMMAND cbtest)

add_custom_target(run_tests COMMAND cbtest -r junit DEPENDS cbtest)
