project(polyline VERSION 1.0.0 LANGUAGES CXX)

set(SRC_FILES
	main.cpp
	util.cpp
	util.h
	app.cpp
	app.h
	types.h
	viewport.h
	drawer/polyline_drawer.cpp
	drawer/polyline_drawer.h
	drawer/old_quad_drawer.cpp
	drawer/old_quad_drawer.h
	drawer/quad_drawer.cpp
	drawer/quad_drawer.h
	drawer/solid_polyline_drawer.cpp
	drawer/solid_polyline_drawer.h
	)
set(LIBRARIES
	glad
	glfw
	)

add_executable(${PROJECT_NAME} ${SRC_FILES})
target_link_libraries(${PROJECT_NAME} ${LIBRARIES})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)
set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${POLYLINE_WORKING_DIRECTORY})

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)