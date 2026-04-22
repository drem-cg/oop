cmake_minimum_required(VERSION 3.16)
project(Lab5_CDate LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTS "Build Google Tests" ON)

add_library(Logger STATIC src/Logger/Logger.cpp)
target_include_directories(Logger PUBLIC include)

add_library(CDate STATIC src/CDate.cpp)
target_include_directories(CDate PUBLIC include)
target_link_libraries(CDate PRIVATE Logger)

add_executable(app main.cpp)
target_link_libraries(app PRIVATE CDate Logger)

if (BUILD_TESTS)
    find_package(GTest REQUIRED)
    enable_testing()

    add_executable(CDateTests tests/CDateTests.cpp
            tests/CDateIssTests.cpp)
    target_link_libraries(CDateTests PRIVATE CDate Logger GTest::gtest_main)
    add_test(NAME CDateTests COMMAND CDateTests)
endif ()