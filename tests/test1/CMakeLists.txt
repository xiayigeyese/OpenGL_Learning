project(test1)

file(GLOB_RECURSE SRCS
		"${PROJECT_SOURCE_DIR}/*.cpp"
		"${PROJECT_SOURCE_DIR}/*.h"
		"${PROJECT_SOURCE_DIR}/shaders/*.vert"
		"${PROJECT_SOURCE_DIR}/shaders/*.frag")
		
add_executable(test1 ${SRCS})

target_link_libraries(test1 PUBLIC opengl)

set(test1_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/test1/")
set_target_properties(test1 
	PROPERTIES 
	RUNTIME_OUTPUT_DIRECTORY ${test1_RUNTIME_OUTPUT_DIRECTORY}
)
# COPY_ASSIMP_BINARIES(${test1_RUNTIME_OUTPUT_DIRECTORY})

# set workDir
if(MSVC)
	set_target_properties(test1 PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
endif()	