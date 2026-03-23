cmake_minimum_required(VERSION 3.10)
project(PrimeGenerator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(prime_generator
        src/main.cpp
        src/prime_generator.cpp
)

enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(prime_generator_test
        tests/prime_generator_test.cpp
        src/prime_generator.cpp
)
target_link_libraries(prime_generator_test ${GTEST_LIBRARIES} pthread)

add_test(NAME PrimeGeneratorTest COMMAND prime_generator_test)