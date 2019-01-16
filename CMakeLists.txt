cmake_minimum_required(VERSION 3.0)

project(PhongViewer)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

### Making sure I have all that I need
find_package(OpenGL REQUIRED)

find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})

find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIRS})

# Getting glfw3: what a fun!

# Required for glfw3 
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)


# Now we have to guess: what do we have, static or dynamic libraries?
if(NOT ${GLFW_LIBRARIES})
    set(GLFW_LIBRARIES ${GLFW_STATIC_LIBRARIES})
endif()


# CMAKE_SOURCE_DIR points to the dir that contains the CMakeLists.txt file
set(PHONGVIEWER_DIR "${CMAKE_SOURCE_DIR}") 

include_directories(${PHONGVIEWER_DIR}/include)

include_directories(${PHONGVIEWER_DIR}/src/tucano)

set(
  PHONGVIEWER_SOURCES
  ${PHONGVIEWER_DIR}/src/main.cpp
  ${PHONGVIEWER_DIR}/src/main_window.cpp
  ${PHONGVIEWER_DIR}/src/simple_widget.cpp
  )

add_executable(phongViewer ${PHONGVIEWER_SOURCES})

set_property(
  TARGET phongViewer
  PROPERTY COMPILE_FLAGS
  "-Wall -g -std=c++11"
  )

target_link_libraries( phongViewer
  ${GLEW_LIBRARIES}
  ${GLFW_LIBRARIES}
  ${OPENGL_LIBRARIES}
  )

add_custom_command( TARGET phongViewer 
    POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/src/tucano/tucano/effects/shaders 
        ${CMAKE_CURRENT_BINARY_DIR}/shaders 
  )
