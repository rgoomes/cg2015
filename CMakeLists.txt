cmake_minimum_required(VERSION 2.8)
# Project Name
PROJECT(HW_OPENGL)

find_package(GLUT REQUIRED)
find_package(Threads)
include_directories(${GLUT_INCLUDE_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})
add_definitions(${GLUT_DEFINITIONS})
if(NOT GLUT_FOUND)
    message(ERROR " GLUT not found!")
endif(NOT GLUT_FOUND)

find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)

find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    include_directories(${GLEW_INCLUDE_DIRS})
    link_libraries(${GLEW_LIBRARIES})
endif()

find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${GLFW_INCLUDE_DIRS})

find_package(Bullet REQUIRED)
if(${BULLET_FOUND})
	include_directories(${BULLET_INCLUDE_DIRS})
	link_libraries(${BULLET_LIBRARIES})
endif()

#########################################################
# Include Files
#########################################################
set(CMAKE_CXX_FLAGS "-DGL_GLEXT_PROTOTYPES -Wall -std=c++11")
file(GLOB src
    "*.h"
    "*.cpp"
)

include_directories (objects)

add_executable(main ${src})

########################################################
# Linking & stuff
#########################################################

file (GLOB_RECURSE PROJECT_SOURCES "*.cpp")

target_link_libraries(main ${GLFW_LIBRARIES} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${CMAKE_THREAD_LIBS_INIT} ${EXTRA_FLAGS} ${BULLET_LIBRARIES} )
