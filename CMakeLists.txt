cmake_minimum_required(VERSION 2.8)

project(sterra_task_stl C CXX)

find_package ( Threads REQUIRED )

add_executable(${PROJECT_NAME} "main.cpp"
    "sterragiven.h"
    "sterragiven.cpp"
    "helpers.h"
    "helpers.cpp"
    )

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        COMPILE_OPTIONS "-Wpedantic;-Wall;-Wextra"
        )
endif()

target_link_libraries (${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
