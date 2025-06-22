# CMakeLists file for polyline

cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

add_subdirectory(deps)

project(polyline VERSION 1.0.0 LANGUAGES CXX)

set(SRC_FILES
	src/main.cpp
	src/util.cpp
	src/util.h
	src/app.cpp
	src/app.h
	src/drawer.cpp
	src/drawer.h
	src/types.h
	src/viewport.h
	)
set(CAPS_LIBRARIES
	glad
	glfw
	)

add_executable(${PROJECT_NAME} ${SRC_FILES})
target_link_libraries(${PROJECT_NAME} ${CAPS_LIBRARIES})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)
# Add MSVC compiler option to enable Structured exception handling (SEH)
if (MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE "/EHa")
endif()