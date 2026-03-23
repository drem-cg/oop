cmake_minimum_required(VERSION 3.10)
project(WordFrequencyCounter)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_library(WordFrequencyCounterCore
        word_counter.cpp
        word_counter.h
)

add_executable(WordFrequencyCounterApp
        main.cpp
)

add_executable(WordFrequencyCounterTests
        tests.cpp
        word_counter.cpp
        word_counter.h
)

target_link_libraries(WordFrequencyCounterTests GTest::Main WordFrequencyCounterCore)


enable_testing()
add_test(NAME WordFrequencyCounterTests
        COMMAND WordFrequencyCounterTests
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)