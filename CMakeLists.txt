cmake_minimum_required(VERSION 3.31)

project(meta-bfield)
enable_language(CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(bfield OBJECT
	bfield.cpp
	bfield_builder.cpp
)

add_executable(meta_bfield
	meta_bfield.cpp
)
target_link_libraries(meta_bfield bfield)

enable_testing()
add_subdirectory(third_party/googletest EXCLUDE_FROM_ALL)
add_subdirectory(tests)
