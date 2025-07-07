project(performance_test VERSION 1.0.0 LANGUAGES CXX)

set(SRC_FILES
	main.cpp
	time_elapsed_query.cpp
	time_elapsed_query.h
	../common/util.cpp
	../common/util.h
	../common/types.h
	../common/viewport.h
	../polyline/drawer/old_quad_drawer.cpp
	../polyline/drawer/old_quad_drawer.h
	../polyline/drawer/quad_drawer.cpp
	../polyline/drawer/quad_drawer.h
	)
set(INCLUDES
	../common
	../polyline
	)
set(LIBRARIES
	glad
	glfw
	)

add_executable(${PROJECT_NAME} ${SRC_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDES})
target_link_libraries(${PROJECT_NAME} ${LIBRARIES})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)
set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${POLYLINE_WORKING_DIRECTORY})

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)