# Specify the minimum version for CMake

cmake_minimum_required(VERSION 2.8)

# Project's name

project(lexer)

# Set the output folder where your program will be created
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set(project_SOURCES
  ${PROJECT_SOURCE_DIR}/lexer.c
  ${PROJECT_SOURCE_DIR}/utils.c
  ${PROJECT_SOURCE_DIR}/main.c
)
add_executable(lexer ${project_SOURCES})

include_directories("${PROJECT_SOURCE_DIR}")
