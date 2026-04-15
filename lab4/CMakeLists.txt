cmake_minimum_required(VERSION 3.14)
project(Lab4Shapes LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif ()

find_package(GTest REQUIRED)
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

set(FAKEIT_DIR "${CMAKE_SOURCE_DIR}/FakeIt/single_header/gtest")

set(APP_SOURCES
        src/Logger/Logger.cpp
        src/ShapeParser/ShapeParser.cpp
        src/ShapeAnalyzer/ShapeAnalyzer.cpp
        src/LineSegment/CLineSegment.cpp
        src/Triangle/CTriangle.cpp
        src/Rectangle/CRectangle.cpp
        src/Circle/CCircle.cpp
        src/Canvas/CCanvas.cpp
        src/main.cpp
)

set(TEST_SOURCES
        src/Logger/Logger.cpp
        src/ShapeParser/ShapeParser.cpp
        src/ShapeAnalyzer/ShapeAnalyzer.cpp
        src/LineSegment/CLineSegment.cpp
        src/Triangle/CTriangle.cpp
        src/Rectangle/CRectangle.cpp
        src/Circle/CCircle.cpp
        tests/shape_test.cpp
        tests/canvas_drawable_test.cpp
        tests/stdin_test.cpp
        tests/stdin_test.cpp
)

add_executable(shape_app ${APP_SOURCES})
target_include_directories(shape_app PRIVATE src)
target_compile_options(shape_app PRIVATE -Wall -Wextra -pedantic)
target_link_libraries(shape_app PRIVATE sfml-graphics sfml-window sfml-system)

add_executable(shape_tests ${TEST_SOURCES})
target_include_directories(shape_tests PRIVATE src ${FAKEIT_DIR})
target_link_libraries(shape_tests PRIVATE GTest::gtest GTest::gtest_main pthread)

enable_testing()
add_test(NAME ShapeTests COMMAND shape_tests)