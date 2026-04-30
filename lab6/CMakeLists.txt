cmake_minimum_required(VERSION 3.16)
project(HttpUrlParser LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(HttpUrlLib STATIC
        src/HttpUrl.cpp
)
target_include_directories(HttpUrlLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(HttpUrlApp app/main.cpp)
target_link_libraries(HttpUrlApp PRIVATE HttpUrlLib)

find_package(GTest REQUIRED)
add_executable(HttpUrlTests tests/HttpUrlTest.cpp)
target_include_directories(HttpUrlTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(HttpUrlTests PRIVATE HttpUrlLib GTest::GTest GTest::Main)

include(GoogleTest)
gtest_discover_tests(HttpUrlTests)