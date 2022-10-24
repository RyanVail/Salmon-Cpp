cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_REQUIRED True)

set(CMAKE_BUILD_TYPE Debug)

project(Salmon)

include_directories(include)

add_executable(Salmon
	src/symbol_table.cpp
	src/debug.cpp
	src/var_types.cpp
	src/intermediate/intermediate.cpp
	src/main.cpp
	src/intermediate/postprocessor.cpp
	src/asm/aarch32_asm.cpp
) # ^^^^^^^^^^^^^^^^^^^^^^^ These should change based on the compilation target