cmake_minimum_required(VERSION 2.8)
project(modelspace)

# Ignore tolua for now.

add_subdirectory(../external/lua/src ${CMAKE_BINARY_DIR}/lib/lua)
add_subdirectory(../external/glm     ${CMAKE_BINARY_DIR}/lib/glm)
add_subdirectory(../external/glfw-3.1.1 ${CMAKE_BINARY_DIR}/lib/glfw)
add_subdirectory(../external/glew-1.12.0 ${CMAKE_BINARY_DIR}/lib/glew)
add_subdirectory(../external/cJSON ${CMAKE_BINARY_DIR}/lib/cJSON)
add_subdirectory(../external/lpng ${CMAKE_BINARY_DIR}/lib/png)

#aux_source_directory(. MODELTEST_SRC)
include(SourceList)
include(../third/linterop/src/SourceExports)

#############################
#############################
#############################
#############################
#############################
#############################

set(COMM_SRC 
	#../common/controls.cpp
	../common/controls.hpp
	../common/objloader.cpp
	../common/objloader.hpp
	../common/quaternion_utils.cpp
	../common/quaternion_utils.hpp
	../common/shader.cpp
	../common/shader.hpp
	../common/tangentspace.cpp
	../common/tangentspace.hpp
	../common/text2D.cpp
	../common/text2D.hpp
	../common/texture.cpp
	../common/texture.hpp
	../common/vboindexer.cpp
	../common/vboindexer.hpp
)
source_group(common 
	FILES ${COMM_SRC}
)

add_executable(modelspace ${FULL_SRC_LIST} ${COMM_SRC})

include_directories(../external/lua/src)
include_directories(../external/glm)
include_directories(../external/glfw-3.1.1/include/GLFW)
include_directories(../external/glew-1.12.0/include)
include_directories(../external/cJSON)
include_directories(../)
include_directories(../source)
include_directories(../external/lpng)
include_directories(${CMAKE_BINARY_DIR}/lib/png)

target_link_libraries(modelspace liblua)
target_link_libraries(modelspace libglew_shared)
target_link_libraries(modelspace glfw)
target_link_libraries(modelspace libcjson)
target_link_libraries(modelspace png16)


############################
#Define macros of my own
macro(ADD_FRAMEWORK fwname appname)
    find_library(FRAMEWORK_${fwname}
    	NAMES ${fwname}
    	PATHS ${CMAKE_OSX_SYSROOT}/System/Library
    	PATH_SUFFIXES Frameworks
    	NO_DEFAULT_PATH)
    if( ${FRAMEWORK_${fwname}} STREQUAL FRAMEWORK_${fwname}-NOTFOUND)
    	#SEND_ERROR must be called. (stop generating)
        message(SEND_ERROR ": Framework ${fwname} not found")
    else()
        target_link_libraries(${appname} "${FRAMEWORK_${fwname}}/${fwname}")
        message(STATUS "Framework ${fwname} found at ${FRAMEWORK_${fwname}}")
    endif()
endmacro(ADD_FRAMEWORK)

###### Some must be appended to Xcode project.
if(APPLE)
	message(STATUS "Searching framework in: ${CMAKE_OSX_SYSROOT}/System/Library")
	set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++11")
	message(STATUS "C++ Dialect to C++ 11")
	#ADD_FRAMEWORK(IOKit modelspace)
	#ADD_FRAMEWORK(CoreVideo modelspace)
	#ADD_FRAMEWORK(Cocoa modelspace)
	#ADD_FRAMEWORK(OpenGL modelspace)
endif()


set_target_properties(modelspace
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/output"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/output"
)


