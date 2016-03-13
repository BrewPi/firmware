
# retrieve the source and include paths for the library
GET_PROPERTY(cbox_lib_src GLOBAL PROPERTY cbox_lib_src)
GET_PROPERTY(cbox_lib_inc GLOBAL PROPERTY cbox_lib_inc)

# convert from a single string to a list
separate_arguments(cbox_lib_src)
separate_arguments(cbox_lib_inc)

set(example_shared_src ${cbox_examples}/shared/timems.cpp)

add_executable(example_non_static main.cpp ${cbox_lib_src} ${example_shared_src})
target_compile_definitions(example_non_static PUBLIC CONTROLBOX_STATIC=0)

target_include_directories(example_non_static PUBLIC ${cbox_lib_inc} ../shared)
target_include_directories(example_non_static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
