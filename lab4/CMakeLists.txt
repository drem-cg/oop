cmake_minimum_required(VERSION 3.14)
project(Lab4Shapes LANGUAGES CXX)

add_compile_options(-march=x86-64 -mtune=generic)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif ()

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set(APP_SOURCES
        src/Logger.cpp
        src/ShapeBase.cpp
        src/Rectangle.cpp
        src/Circle.cpp
        src/Triangle.cpp
        src/Ellipse.cpp
        src/ShapeParser.cpp
        src/ShapeAnalyzer.cpp
        src/main.cpp
)

set(TEST_SOURCES
        src/Logger.cpp
        src/ShapeBase.cpp
        src/Rectangle.cpp
        src/Circle.cpp
        src/Triangle.cpp
        src/Ellipse.cpp
        src/ShapeParser.cpp
        src/ShapeAnalyzer.cpp
        tests/shape_test.cpp
)

add_executable(shape_app ${APP_SOURCES})
target_include_directories(shape_app PRIVATE src)
target_compile_options(shape_app PRIVATE -Wall -Wextra -pedantic)

add_executable(shape_tests ${TEST_SOURCES})
target_include_directories(shape_tests PRIVATE src)
target_link_libraries(shape_tests PRIVATE GTest::gtest GTest::gtest_main pthread)

enable_testing()
add_test(NAME ShapeTests COMMAND shape_tests)