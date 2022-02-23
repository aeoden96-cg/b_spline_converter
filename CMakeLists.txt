cmake_minimum_required(VERSION 3.16)
project(PROJECT)
set(CMAKE_CXX_STANDARD 17)


#find_package(OpenGL)
#find_package(GLUT)
#find_package(GLEW)

add_library(SHADER      util/Shader.cpp util/Shader.h)
set_target_properties(SHADER    PROPERTIES PUBLIC_HEADER util/Shader.h)
target_include_directories (SHADER PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

ADD_EXECUTABLE(PROJECT
        source/main.cpp
        source/main.hpp
        source/Background.cpp
        source/Background.hpp
        source/Renderer.cpp
        source/Renderer.hpp)

#include_directories(
#${OPENGL_INCLUDE_DIRS}
#${GLUT_INCLUDE_DIRS}
#${GLEW_INCLUDE_DIRS})

target_link_libraries(
            PROJECT LINK_PUBLIC
            SHADER
#            ${OPENGL_LIBRARIES}
#            ${GLUT_LIBRARY}
#            ${GLEW_LIBRARIES}
        -lGLEW -lGL -lGLU -lglut -lpthread -lm)

file(GLOB MY_SHADERS "source/*.tesc" "source/*.tese" "source/*.vert" "source/*.frag" "source/*.geom" "source/*.yaml")
foreach(shader ${MY_SHADERS})
    configure_file(${shader} ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
endforeach()


#add_subdirectory(  "${PROJECT_SOURCE_DIR}/source" )
