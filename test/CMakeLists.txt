cmake_minimum_required (VERSION 3.1)
set (CMAKE_CXX_STANDARD 11)
project (cbtest)


file(GLOB test_SRC
    "*.cpp"
)
add_executable(cbtest ${test_SRC})

