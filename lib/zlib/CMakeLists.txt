cmake_minimum_required(VERSION 3.8)

project(zlib C)

include("CMakeCompileDefinitions.txt")

add_library(zlib
	"src/zutil.c"
	"src/inflate.c"
	"src/adler32.c"
	"src/inffast.c"
	"src/inftrees.c"
)
target_include_directories(zlib PRIVATE "inc")