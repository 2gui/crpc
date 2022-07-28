
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(crpc VERSION 0.1.0 LANGUAGES C)


file(GLOB_RECURSE headers ./src/*.h)
file(GLOB_RECURSE sources ./src/*.c)

message(STATUS "headers: ${headers}")
message(STATUS "sources: ${sources}")

include_directories(./src)
add_library(Crpc STATIC ${sources})

set(BUILD_EXAMPLE on)
# option(BUILD_EXAMPLE "build example" ON)

if(${BUILD_EXAMPLE})
	add_subdirectory(_example)
endif()
