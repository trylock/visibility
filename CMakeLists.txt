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

include_directories(${PROJECT_SOURCE_DIR})

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Werror -Wextra")
endif()

# Create project in MSVC
add_library(visibility STATIC ${visibility_headers})
set_target_properties(visibility PROPERTIES LINKER_LANGUAGE CXX) 

add_executable(tests ${all_tests})