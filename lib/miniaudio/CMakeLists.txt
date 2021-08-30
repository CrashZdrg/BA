cmake_minimum_required(VERSION 3.8)

project(miniaudio C)

include("CMakeCompileDefinitions.txt")
add_compile_definitions(MINIAUDIO_IMPLEMENTATION)

add_library(miniaudio "src/miniaudio.c")
target_include_directories(miniaudio PRIVATE "inc")

if(UNIX)
	target_link_libraries(miniaudio dl pthread)
endif()