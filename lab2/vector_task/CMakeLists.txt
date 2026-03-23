cmake_minimum_required(VERSION 3.10)
project(VectorTask)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_library(vector_core
        vector.cpp
        vector.h
)

add_executable(vector_app
        main.cpp
)
target_link_libraries(vector_app vector_core)

add_executable(vector_tests
        vector_tests.cpp
)
target_link_libraries(vector_tests GTest::Main vector_core)

add_custom_target(test
        COMMAND vector_tests
        DEPENDS vector_tests
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)