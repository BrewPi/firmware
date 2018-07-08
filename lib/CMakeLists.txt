cmake_minimum_required (VERSION 2.6)
project (CatchTestPlayground)
set (CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -Wno-unused-parameter")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_SYS_WAIT_H_ -D_SYS_WAIT_H")
add_subdirectory("catch-test")

enable_testing()