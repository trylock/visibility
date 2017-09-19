cmake_minimum_required(VERSION 2.8.0)
project(visibility)

set(visibility_headers 
    ${PROJECT_SOURCE_DIR}/visibility/floats.hpp
    ${PROJECT_SOURCE_DIR}/visibility/vector2.hpp
    ${PROJECT_SOURCE_DIR}/visibility/primitives.hpp
    ${PROJECT_SOURCE_DIR}/visibility/visibility.hpp
)

set(all_tests
    ${PROJECT_SOURCE_DIR}/tests/catch.hpp
    ${PROJECT_SOURCE_DIR}/tests/main.cpp
    ${PROJECT_SOURCE_DIR}/tests/vector2_test.cpp
    ${PROJECT_SOURCE_DIR}/tests/primitives_test.cpp
    ${PROJECT_SOURCE_DIR}/tests/visibility_test.cpp
)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Werror -Wextra")
endif()

include_directories(${PROJECT_SOURCE_DIR})

add_executable(tests ${all_tests})