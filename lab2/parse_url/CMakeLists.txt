cmake_minimum_required(VERSION 3.10)
project(ParseURL)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(parse_url
        src/main.cpp
        src/parse_url.cpp
)

enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(parse_url_tests
        tests/parse_url_tests.cpp
        src/parse_url.cpp
)
target_link_libraries(parse_url_tests ${GTEST_LIBRARIES} pthread)

add_test(NAME ParseURLTest COMMAND parse_url_tests)