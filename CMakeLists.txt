cmake_minimum_required(VERSION 3.22)
project(MPCS51045_Raytracer)

set(CMAKE_CXX_STANDARD 20)

if (APPLE)
    add_definitions(-DCL_HPP_TARGET_OPENCL_VERSION=120 -DCL_HPP_MINIMUM_OPENCL_VERSION=120)
endif()

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl3)

file(GLOB CPP_SOURCE "${PROJECT_SOURCE_DIR}/src/*")
include_directories("${PROJECT_SOURCE_DIR}/include")

find_package(SDL3 REQUIRED)
include_directories(${SDL3_INCLUDE_DIRS}) 

add_executable(MPCS51045_Raytracer ${CPP_SOURCE})

target_link_libraries(${PROJECT_NAME} ${SDL3_LIBRARIES})
target_link_libraries(${PROJECT_NAME} "-framework OpenCL")
