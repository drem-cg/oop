cmake_minimum_required(VERSION 3.10)
project(CalculatorProject)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(calc_app
        src/main.cpp
        src/Calculator.h
        src/Calculator.cpp
        src/Variable.h
        src/Variable.cpp
        src/Function.h
        src/Function.cpp
        src/Parser.h
        src/Parser.cpp
        src/Logger.h
        src/Logger.cpp
        src/Calculator.cpp
        src/Calculator.h
        src/Variable.cpp
        src/Variable.h
        src/Function.cpp
        src/Function.h
        src/Parser.cpp
        src/Parser.h
        src/Logger.cpp
        src/Logger.h
)

add_executable(calc_tests
        tests/Variable_test.cpp
        tests/Function_test.cpp
        tests/Calculator_test.cpp
        tests/Parser_test.cpp
        src/Variable.cpp
        src/Function.cpp
        src/Calculator.cpp
        src/Parser.cpp
        src/Logger.cpp
        tests/Variable_test.cpp
        tests/Function_test.cpp
        tests/Calculator_test.cpp
        tests/Parser_test.cpp
        tests/CommandLine_test.cpp
)

target_link_libraries(calc_tests GTest::Main)

add_custom_target(test
        COMMAND calc_tests
        DEPENDS calc_tests
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)