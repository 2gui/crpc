
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

set(PROJECT_NAME Crpc)
set(PROJECT_VERSION 0.1.0)
project(${PROJECT_NAME} VERSION ${PROJECT_VERSION} LANGUAGES C)

# set(CMAKE_BUILD_TYPE debug)
string(TOUPPER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE)
if("${CMAKE_BUILD_TYPE}" STREQUAL "DEBUG")
	set(DEBUG on)
	set(CMAKE_C_FLAGS "-O0")
endif()

set(INSTALL_PACKAGE on)
set(BUILD_EXAMPLE on)

file(GLOB_RECURSE cinfiles ./src/*.cin)
foreach(f ${cinfiles})
	string(LENGTH ${f} t)
	math(EXPR t "${t} - 4")
	string(SUBSTRING ${f} 0 ${t} t)
	message(STATUS "configured file: ${f} => ${t}")
	configure_file(${f} ${t})
endforeach()

file(GLOB_RECURSE headers ./src/*.h)
file(GLOB_RECURSE sources ./src/*.c)

message(STATUS "headers: ${headers}")
message(STATUS "sources: ${sources}")

include_directories(./includes)
include_directories(./src)
add_library(${PROJECT_NAME} STATIC ${sources})

if(${INSTALL_PACKAGE})
	configure_file(Config.cmake.cin CrpcConfig.cmake @ONLY)
	install(FILES ./build/CrpcConfig.cmake DESTINATION lib/cmake/Crpc)
	install(FILES ${headers} DESTINATION include/Crpc)
	install(FILES includes/Crpc.h DESTINATION include)
	install(TARGETS ${PROJECT_NAME})
endif()

if(${BUILD_EXAMPLE})
	add_subdirectory(_example)
endif()
