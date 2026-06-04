cmake_minimum_required(VERSION 3.31)

project(meta-bfield)
enable_language(CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Boost REQUIRED COMPONENTS json)

add_library(bfield OBJECT
    src/bexport.cpp
    src/bfield.cpp
    src/bfield_builder.cpp
    src/bpart.cpp
    src/c_generator.cpp
    src/spec.cpp
    src/util.cpp
)
target_link_libraries(bfield Boost::boost Boost::json)
target_include_directories(bfield INTERFACE src)

add_executable(meta_bfield
    src/meta_bfield.cpp
)
target_link_libraries(meta_bfield bfield)

if(false)
enable_testing()
add_subdirectory(third_party/googletest EXCLUDE_FROM_ALL)
add_subdirectory(tests)
endif()
