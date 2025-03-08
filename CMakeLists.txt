cmake_minimum_required(VERSION 3.31.3)

Project(Advanced-types)

file(GLOB SOURCES "source/*.c")
add_library(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC headers)

