cmake_minimum_required(VERSION 3.8)

project(BA)

option(NOAUDIO "Compile without audio" OFF)

if(NOAUDIO)
	add_compile_definitions (NOAUDIO)
else()
	add_subdirectory ("lib/miniaudio")
endif()

option(USE_SPINWAIT "Use an alternative rendering loop" OFF)
if(USE_SPINWAIT)
	add_compile_definitions(USE_SPINWAIT)
endif()

option(NOINTRO "Disable the intro sequence" OFF)
if(NOINTRO)
	add_compile_definitions(NOINTRO)
endif()

add_subdirectory("lib/zlib")

add_subdirectory("BA")

