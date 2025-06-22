# CMakeLists file for glad

project(glad LANGUAGES C)

set(SRC_FILES
	src/glad.c
	)

add_library(${PROJECT_NAME} STATIC ${SRC_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)