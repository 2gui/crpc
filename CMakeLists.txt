
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

set(PROJECT_NAME Crpc)
set(PROJECT_VERSION 0.1.0)
project(${PROJECT_NAME} VERSION ${PROJECT_VERSION} LANGUAGES C)

set(INSTALL_PACKAGE on)
set(BUILD_EXAMPLE on)

file(GLOB_RECURSE headers ./src/*.h)
file(GLOB_RECURSE sources ./src/*.c)

message(STATUS "headers: ${headers}")
message(STATUS "sources: ${sources}")

include_directories(./includes)
include_directories(./src)
add_library(${PROJECT_NAME} STATIC ${sources})

if(${INSTALL_PACKAGE})
	configure_file(Config.cmake.in CrpcConfig.cmake @ONLY)
	install(FILES ./build/CrpcConfig.cmake DESTINATION lib/cmake/Crpc)
	install(FILES ${headers} DESTINATION include/Crpc)
	install(FILES includes/Crpc.h DESTINATION include)
	install(TARGETS ${PROJECT_NAME})
endif()

if(${BUILD_EXAMPLE})
	add_subdirectory(_example)
endif()
