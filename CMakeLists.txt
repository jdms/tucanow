###############################################
# Preliminaries  
###############################################

cmake_minimum_required(VERSION 3.0)

project(PhongViewer)

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

# Eigen3 by Tucano
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
# Working with the project files
###############################################

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -O2") 

set(PHONGVIEWER_DIR "${CMAKE_SOURCE_DIR}") 

include_directories(${PHONGVIEWER_DIR}/include)
include_directories(${PHONGVIEWER_DIR}/src/tucano)

set(PHONGVIEWER_SOURCES
  ${PHONGVIEWER_DIR}/src/main.cpp
  ${PHONGVIEWER_DIR}/src/main_window.cpp
  ${PHONGVIEWER_DIR}/src/simple_widget.cpp
  )

add_executable(phongViewer ${PHONGVIEWER_SOURCES})


###############################################
# Copy shaders to binary dir when done
###############################################

add_custom_command(TARGET phongViewer POST_BUILD 
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/src/tucano/tucano/effects/shaders 
        ${CMAKE_CURRENT_BINARY_DIR}/shaders 
  )
