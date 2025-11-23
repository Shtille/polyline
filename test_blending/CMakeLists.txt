project(test_blending VERSION 1.0.0 LANGUAGES CXX)

set(SRC_FILES
	main.cpp
	app.cpp
	app.h
	../common/base_app.cpp
	../common/base_app.h
	../common/util.cpp
	../common/util.h
	../common/camera.cpp
	../common/camera.h
	../common/types.h
	../common/viewport.h
	../common/input_controller.cpp
	../common/input_controller.h
	../common/orbit_controller.cpp
	../common/orbit_controller.h
	drawer/square_drawer.cpp
	drawer/square_drawer.h
	drawer/screen_drawer.cpp
	drawer/screen_drawer.h
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