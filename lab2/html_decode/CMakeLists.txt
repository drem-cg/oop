cmake_minimum_required(VERSION 3.10)
project(HtmlDecode)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_library(html_core STATIC
        src/html_decode.cpp
        src/html_decode.h
)

add_executable(html_decode src/main.cpp)
target_link_libraries(html_decode html_core)

add_executable(html_decode_tests tests/html_decode_test.cpp)
target_link_libraries(html_decode_tests GTest::gtest_main html_core)

include(GoogleTest)
add_test(
        NAME html_decode_tests
        COMMAND html_decode_tests
)