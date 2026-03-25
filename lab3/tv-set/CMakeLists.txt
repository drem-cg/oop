cmake_minimum_required(VERSION 3.10)
project(TVSetProject)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(tv_app
        src/main.cpp
        src/TVSet.h
        src/TVSet.cpp
        src/RemoteControl.h
        src/RemoteControl.cpp
)

add_executable(tv_tests
        tests/TVSet_test.cpp
        src/TVSet.cpp
        src/RemoteControl.cpp
)

target_link_libraries(tv_tests GTest::Main)

add_custom_target(test
        COMMAND tv_tests
        DEPENDS tv_tests
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)