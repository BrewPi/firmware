cmake_minimum_required (VERSION 3.4.3)
project (controlbox)
set(CMAKE_CXX_STANDARD 11)

set(project_dir ${CMAKE_CURRENT_SOURCE_DIR})
set(cbox_examples ${project_dir}/examples)


add_subdirectory(src)
add_subdirectory(examples)
add_subdirectory(test)

