###############################################
# Preamble  
###############################################

cmake_minimum_required(VERSION 3.0)

project(MeshViewer)

# For vim's YouCompleteMe plugin
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(TUCANOW_BUILD_SHARED_LIBRARY "Build tucanow as a shared library" ON)

if(TUCANOW_BUILD_SHARED_LIBRARY)
    set(TUCANOW_LIBRARY_TYPE "SHARED")
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

###############################################
# External libraries
###############################################

find_package(OpenGL REQUIRED)

# GLEW is required by Tucano
find_package(GLEW REQUIRED)

# Eigen3 is required by Tucano
find_package(Eigen3 REQUIRED)


# ###############################################
# # Making glfw3 work is more fun! 
# ###############################################

# # As per its docs, glfw3 requires the use of 
# # pkg-config (see `man pkg-config`)

# find_package(PkgConfig REQUIRED)

# pkg_search_module(GLFW REQUIRED glfw3)
# include_directories(${GLFW_INCLUDE_DIRS})

# # What do we have, static or dynamic libraries?
# if(NOT GLFW_LIBRARIES)
#     set(GLFW_LIBRARIES ${GLFW_STATIC_LIBRARIES})
# endif()
# link_libraries(${GLFW_LIBRARIES})


###############################################
# Include project's files and build binary
###############################################


set(TUCANOW_SOURCES
    # ${MESH_VIEWER_DIR}/src/main_window.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/scene.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/gui.cpp
  )

add_library(tucanow ${TUCANOW_LIBRARY_TYPE} ${TUCANOW_SOURCES})
add_library(tucanow::tucanow ALIAS tucanow)

target_include_directories(tucanow
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/src/tucano
)

target_link_libraries(tucanow GLEW::GLEW OpenGL::OpenGL Eigen3::Eigen)

target_compile_features(tucanow PUBLIC cxx_std_11)

# MSVC (as recently as version 19.15.26729) throws hundreds of warnings from
# it's own implementation of the STL -- CMake's SYSTEM directive seems
# ineffective to prevent this behaviour.  Thus, using -Wall in MSVC is,
# unfortunately, not possible.  

if(NOT MSVC) 
    target_compile_options(tucanow PRIVATE -Wall) 
endif()

# add_executable(mesh_viewer ${MESH_VIEWER_SOURCES} ${MESH_VIEWER_DIR}/src/main_load_ply.cpp)
# add_executable(mesh_viewer_spheres ${MESH_VIEWER_SOURCES} ${MESH_VIEWER_DIR}/src/main_spheres.cpp)


###############################################
# Copy shaders and assets to binary dir when done
###############################################

add_custom_command(TARGET tucanow POST_BUILD 
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/src/tucano/tucano/effects/shaders 
        ${CMAKE_BINARY_DIR}/shaders
)

add_custom_command(TARGET tucanow POST_BUILD 
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/src/tucano/samples/assets
        ${CMAKE_BINARY_DIR}/samples/assets
)

# add_custom_command(TARGET tucanow POST_BUILD 
#     COMMAND ${CMAKE_COMMAND} -E copy_directory
#     ${CMAKE_CURRENT_SOURCE_DIR}/src/tucano/samples/models
#         ${CMAKE_CURRENT_BINARY_DIR}/samples/models
#   )
