###############################################
# Preamble  
###############################################

cmake_minimum_required(VERSION 3.0)

project(MeshViewer)

# For vim's YouCompleteMe plugin
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


###############################################
# External libraries
###############################################

find_package(OpenGL REQUIRED)
link_libraries(${OPENGL_LIBRARIES})

# GLEW is required by Tucano
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
link_libraries(${GLEW_LIBRARIES})

# Eigen3 is required by Tucano
find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIRS})


###############################################
# Making glfw3 work is more fun! 
###############################################

# As per its docs, glfw3 requires the use of 
# pkg-config (see `man pkg-config`)

find_package(PkgConfig REQUIRED)

pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${GLFW_INCLUDE_DIRS})

# What do we have, static or dynamic libraries?
if(NOT GLFW_LIBRARIES)
    set(GLFW_LIBRARIES ${GLFW_STATIC_LIBRARIES})
endif()
link_libraries(${GLFW_LIBRARIES})


###############################################
# Include project's files and build binary
###############################################

set(CMAKE_CXX_FLAGS 
    "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -ggdb3") 

set(MESH_VIEWER_DIR "${CMAKE_SOURCE_DIR}") 

include_directories(${MESH_VIEWER_DIR}/include)
include_directories(${MESH_VIEWER_DIR}/src)
include_directories(SYSTEM ${MESH_VIEWER_DIR}/src/tucano)

set(MESH_VIEWER_SOURCES
    ${MESH_VIEWER_DIR}/src/main.cpp
    ${MESH_VIEWER_DIR}/src/main_window.cpp
    ${MESH_VIEWER_DIR}/src/scene.cpp
    ${MESH_VIEWER_DIR}/src/gui.cpp
  )

add_executable(mesh_viewer ${MESH_VIEWER_SOURCES})


###############################################
# Copy shaders and assets to binary dir when done
###############################################

add_custom_command(TARGET mesh_viewer POST_BUILD 
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${MESH_VIEWER_DIR}/src/tucano/tucano/effects/shaders 
        ${CMAKE_CURRENT_BINARY_DIR}/shaders 
  )

add_custom_command(TARGET mesh_viewer POST_BUILD 
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${MESH_VIEWER_DIR}/src/tucano/samples/assets
        ${CMAKE_CURRENT_BINARY_DIR}/samples/assets
  )

add_custom_command(TARGET mesh_viewer POST_BUILD 
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${MESH_VIEWER_DIR}/src/tucano/samples/models
        ${CMAKE_CURRENT_BINARY_DIR}/samples/models
  )
