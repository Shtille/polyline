project(polyline VERSION 1.0.0 LANGUAGES CXX)

set(SRC_FILES
	main.cpp
	app.cpp
	app.h
	../common/base_app.cpp
	../common/base_app.h
	../common/util.cpp
	../common/util.h
	../common/types.h
	../common/viewport.h
	drawer/polyline_drawer.cpp
	drawer/polyline_drawer.h
	drawer/old_quad_drawer.cpp
	drawer/old_quad_drawer.h
	drawer/quad_drawer.cpp
	drawer/quad_drawer.h
	drawer/solid_polyline_drawer.cpp
	drawer/solid_polyline_drawer.h
	drawer/dash_polyline_drawer.cpp
	drawer/dash_polyline_drawer.h
	)
set(INCLUDES
	../common
	)
set(LIBRARIES
	glad
	glfw
	glm
	)

add_executable(${PROJECT_NAME} ${SRC_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDES})
target_link_libraries(${PROJECT_NAME} ${LIBRARIES})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)
set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${POLYLINE_WORKING_DIRECTORY})

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)