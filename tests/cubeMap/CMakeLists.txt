project(TestCubeMap)

file(GLOB_RECURSE SRCS
		"${PROJECT_SOURCE_DIR}/*.cpp"
		"${PROJECT_SOURCE_DIR}/*.h"
		"${PROJECT_SOURCE_DIR}/shaders/*.vert"
		"${PROJECT_SOURCE_DIR}/shaders/*.frag")
		
add_executable(TestCubeMap ${SRCS})

target_include_directories(TestCubeMap PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(TestCubeMap PUBLIC opengl)


set(test_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/TestCubeMap/")
set_target_properties(TestCubeMap
	PROPERTIES 
	RUNTIME_OUTPUT_DIRECTORY ${test_RUNTIME_OUTPUT_DIRECTORY}
)
# COPY_ASSIMP_BINARIES(${test_RUNTIME_OUTPUT_DIRECTORY})

# set workDir
if(MSVC)
	set_target_properties(TestCubeMap PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
endif()	