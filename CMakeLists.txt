cmake_minimum_required(VERSION 2.8.0)
project(visibility)

set(visibility_headers 
    ${PROJECT_SOURCE_DIR}/visibility/Floats.hpp
    ${PROJECT_SOURCE_DIR}/visibility/Vector.hpp
    ${PROJECT_SOURCE_DIR}/visibility/Primitives.hpp
    ${PROJECT_SOURCE_DIR}/visibility/Visibility.hpp
)

set(all_tests
    ${PROJECT_SOURCE_DIR}/tests/catch.hpp
    ${PROJECT_SOURCE_DIR}/tests/main.cpp
    ${PROJECT_SOURCE_DIR}/tests/VectorTest.cpp
    ${PROJECT_SOURCE_DIR}/tests/PrimitivesTest.cpp
    ${PROJECT_SOURCE_DIR}/tests/VisibilityTest.cpp
)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Werror -Wextra")
endif()

include_directories(${PROJECT_SOURCE_DIR})

add_executable(tests ${all_tests})
add_library(visibility STATIC ${visibility_headers})
set_target_properties(visibility PROPERTIES LINKER_LANGUAGE CXX)