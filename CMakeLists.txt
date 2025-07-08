# CMakeLists file for polyline

cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

option(BUILD_TESTS "Whether need build tests" ON)

project(polyline_all VERSION 1.0.0 LANGUAGES CXX C)

set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}")
set(POLYLINE_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

add_subdirectory(deps)
add_subdirectory(polyline)
add_subdirectory(wireframe)

if (BUILD_TESTS)
	add_subdirectory(tests)
endif (BUILD_TESTS)