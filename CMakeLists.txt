cmake_minimum_required(VERSION 3.0)

project(PhongViewer)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PHONGVIEWER_DIR "${CMAKE_SOURCE_DIR}")

include_directories(${PHONGVIEWER_DIR}/include)
include_directories(${PHONGVIEWER_DIR}/src/tucano)

find_package(GLEW REQUIRED)
# if(NOT GLEW_FOUND)
#   message(SEND_ERROR "GLEW not found on your system.")
# endif()

find_package(OpenGL REQUIRED)
# if(NOT OPENGL_FOUND)
#   message(SEND_ERROR "OpenGL not found on your system.")
# endif()

find_package(Eigen3 REQUIRED)

# Eigen 3.
# pkg-config is used so we don't hard-code the location of eigen.
find_package(PkgConfig)
# if(NOT PKG_CONFIG_FOUND)
#     set( EIGEN3_INCLUDE_DIRS "$ENV{EIGEN3_INCLUDE_DIR}" )
#     if( NOT EIGEN3_INCLUDE_DIRS )
#     message( SEND_ERROR "pkg-config not found on your system. Please point the environment variable EIGEN3_INCLUDE_DIR to the include directory of your Eigen3 installation.")
# endif()
# else()
#     pkg_check_modules(EIGEN3 REQUIRED eigen3)
# endif()
pkg_search_module(GLFW REQUIRED glfw3)

# find_package(glfw3 REQUIRED)

include_directories(${EIGEN3_INCLUDE_DIRS})

set(GLFW_LIBRARIES ${GLFW_STATIC_LIBRARIES})

set(
  PHONGVIEWER_SOURCES
  ${PHONGVIEWER_DIR}/src/main.cpp
  ${PHONGVIEWER_DIR}/src/main_window.cpp
  ${PHONGVIEWER_DIR}/src/simple_widget.cpp
  )

add_executable(
  phongViewer
  ${PHONGVIEWER_SOURCES}
  #  ${TUCANO_SOURCES}
  )

set(TUCANO_EFFECTS_DIR "${CMAKE_SOURCE_DIR}/src/tucano/tucano/effects")
set_property(
  TARGET phongViewer
  PROPERTY COMPILE_FLAGS
  "-DTUCANOSHADERDIR=${TUCANO_EFFECTS_DIR}/shaders/ -Wall -g -std=c++11"
  )

target_link_libraries(
  phongViewer
  ${GLEW_LIBRARIES}
  ${GLFW_LIBRARIES}
  ${OPENGL_LIBRARIES}
  )
