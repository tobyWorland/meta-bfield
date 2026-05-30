cmake_minimum_required(VERSION 3.31)

project(meta-bfield)
enable_language(CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(meta_bfield
	meta_bfield.cpp
)
