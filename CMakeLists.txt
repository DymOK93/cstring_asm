cmake_minimum_required (VERSION 3.19)
cmake_policy(SET CMP0091 NEW)	# Для CMAKE_MSVC_RUNTIME_LIBRARY
project ("CStringTest")
enable_language(CXX ASM_MASM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/exe")

set(
	CMAKE_MSVC_RUNTIME_LIBRARY 
		"MultiThreaded$<$<CONFIG:Debug>:Debug>"	# static MSVC runtime (/MT)
)	

add_executable (
	CStringTest
		"container_traits.hpp"
		"test_runner.hpp"
		"c_string.hpp"
		"c_string.cpp"
		"c_string.asm"
)

target_compile_definitions(
	CStringTest PRIVATE
		_CRT_SECURE_NO_WARNINGS
)
