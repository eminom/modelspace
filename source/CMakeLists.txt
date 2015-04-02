cmake_minimum_required(VERSION 2.8)
project(modelspace)

# Ignore tolua for now.

add_subdirectory(../external/lua/src ${CMAKE_BINARY_DIR}/lib/lua)
add_subdirectory(../external/glm     ${CMAKE_BINARY_DIR}/lib/glm)
add_subdirectory(../external/glfw-3.1.1 ${CMAKE_BINARY_DIR}/lib/glfw)
add_subdirectory(../external/glew-1.12.0 ${CMAKE_BINARY_DIR}/lib/glew)
add_subdirectory(../external/cJSON ${CMAKE_BINARY_DIR}/lib/cJSON)

#aux_source_directory(. MODELTEST_SRC)
set(SRC_LIST
./AppController.h
./Director.h
./InputHost.h
./InputProcessor.h
./InputStdProcessor.h
./LightProtocol.h
./LuaScriptor.h
./protocols.h
./AppController.cpp
./Director.cpp
./InputProcessor.cpp
./InputStdProcessor.cpp
./LuaScriptor.cpp
./main.cpp
)
set(FULL_SRC_LIST ${FULL_SRC_LIST} ${SRC_LIST})


set(SRC_LIST
./interop/class/exports.h
./interop/class/lextension.h
./interop/class/lextension.c
./interop/class/exports.cpp
)
set(FULL_SRC_LIST ${FULL_SRC_LIST} ${SRC_LIST})
source_group(interop\\class FILES ${SRC_LIST})


set(SRC_LIST
./interop/lua_access/lua_access.h
./interop/lua_access/lua_access_macros.h
./interop/lua_access/refobj.h
./interop/lua_access/refproto.h
./interop/lua_access/refprotobase.h
./interop/lua_access/lua_access.cpp
./interop/lua_access/refobj.cpp
./interop/lua_access/refproto.cpp
./interop/lua_access/refprotobase.cpp
)
set(FULL_SRC_LIST ${FULL_SRC_LIST} ${SRC_LIST})
source_group(interop\\lua_access FILES ${SRC_LIST})

set(SRC_LIST
./interop/lua_core/lua_core.h
./interop/lua_core/mcore.h
./interop/lua_core/lua_core.cpp
./interop/lua_core/mcore.cpp
)
set(FULL_SRC_LIST ${FULL_SRC_LIST} ${SRC_LIST})
source_group(interop\\lua_core FILES ${SRC_LIST})

set(SRC_LIST
./model/ObjectModel.h
./model/ObjectModel.cpp
)
set(FULL_SRC_LIST ${FULL_SRC_LIST} ${SRC_LIST})
source_group(model FILES ${SRC_LIST})

set(SRC_LIST
./nodes/AxisNode.h
./nodes/BlenderNode.h
./nodes/BlenderNormal.h
./nodes/BoxNode.h
./nodes/Common.h
./nodes/CubeNode.h
./nodes/DDNodes.h
./nodes/EsNode.h
./nodes/FieldNode.h
./nodes/GridNode.h
./nodes/LightNode.h
./nodes/MimicNode.h
./nodes/PrimitiveNode.h
./nodes/AxisNode.cpp
./nodes/BlenderNode.cpp
./nodes/BlenderNormal.cpp
./nodes/BoxNode.cpp
./nodes/CubeNode.cpp
./nodes/EsNode.cpp
./nodes/FieldNode.cpp
./nodes/GridNode.cpp
./nodes/LightNode.cpp
./nodes/MimicNode.cpp
./nodes/PrimitiveNode.cpp
)
set(FULL_SRC_LIST ${FULL_SRC_LIST} ${SRC_LIST})
source_group(nodes FILES ${SRC_LIST})

set(SRC_LIST
./nodes/graphics/DDApi.h
./nodes/graphics/DDGraphics.h
./nodes/graphics/DDStruct.h
)
set(FULL_SRC_LIST ${FULL_SRC_LIST} ${SRC_LIST})
source_group(nodes\\graphics FILES ${SRC_LIST})


set(SRC_LIST
./shaderman/exshader.h
./shaderman/shadercache.h
./shaderman/exshader.cc
./shaderman/shadercache.cc
)
set(FULL_SRC_LIST ${FULL_SRC_LIST} ${SRC_LIST})
source_group(shaderman FILES ${SRC_LIST})


set(SRC_LIST
./test/test_lua.cpp
)
set(FULL_SRC_LIST ${FULL_SRC_LIST} ${SRC_LIST})
source_group(test FILES ${SRC_LIST})





#############################
#############################
#############################
#############################
#############################
#############################

set(COMM_SRC 
	../common/controls.cpp
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


target_link_libraries(modelspace liblua)
target_link_libraries(modelspace libglew_static)
target_link_libraries(modelspace glfw)
target_link_libraries(modelspace libcjson)

set_target_properties(modelspace
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/output"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/output"
)


