cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_REQUIRED True)

set(CMAKE_BUILD_TYPE Debug)
# if(NOT CMAKE_BUILD_TYPE)
# 	set(CMAKE_BUILD_TYPE Release)
# 	set(CMAKE_CXX_FLAGS_RELEASE "-O2")
# endif()


project(Salmon)

include_directories(include)

add_executable(Salmon
	src/intermediate/typechecker.cpp
	src/symboltable.cpp
	src/debug.cpp
	src/vartypes.cpp
	src/intermediate/intermediate.cpp
	src/main.cpp
	src/intermediate/postprocessor.cpp
	src/asm/aarch32_asm.cpp
	#src/intermediate/optimization.cpp
	src/asm/aarch32_assembler.c
)
