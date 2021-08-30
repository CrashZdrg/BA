cmake_minimum_required (VERSION 3.8)

add_executable(BA "BA.cpp")

include("../lib/zlib/CMakeCompileDefinitions.txt")
target_include_directories(BA PRIVATE "../lib/zlib/inc")
target_link_libraries(BA zlib)

if(NOT NOAUDIO)
	include("../lib/miniaudio/CMakeCompileDefinitions.txt")
	target_include_directories(BA PRIVATE "../lib/miniaudio/inc")
	target_link_libraries(BA miniaudio)
endif()