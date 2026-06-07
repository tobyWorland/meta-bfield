cmake_minimum_required(VERSION 3.31)

project(meta-bfield)
enable_language(CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Boost REQUIRED COMPONENTS json)

add_subdirectory(preludes)

add_library(bfield OBJECT
    src/bexport.cpp
    src/bexpr.cpp
    src/bfield.cpp
    src/bfield_builder.cpp
    src/bpart.cpp
    src/c_generator.cpp
    src/spec.cpp
    src/util.cpp
)
target_link_libraries(bfield Boost::boost Boost::json preludes)
target_include_directories(bfield INTERFACE src)

add_executable(meta_bfield
    src/meta_bfield.cpp
)
target_link_libraries(meta_bfield bfield)

function (meta_gen name outbase spec_file)
add_custom_command(
    OUTPUT "${CMAKE_BINARY_DIR}/${outbase}.c"
    DEPENDS meta_bfield preludes
    COMMAND "${CMAKE_BINARY_DIR}/meta_bfield"
            "${CMAKE_SOURCE_DIR}/${spec_file}"
            "${CMAKE_BINARY_DIR}/${outbase}"
)
add_library(gen_${name} STATIC
    "${CMAKE_BINARY_DIR}/${outbase}.c"
)
target_include_directories(gen_${name} PUBLIC "${CMAKE_BINARY_DIR}")
endfunction()

meta_gen("t32_encoding" "t32_encoding" "example_specs/t32.json")
meta_gen("example" "example" "example_specs/example.json")

add_subdirectory(demos)

enable_testing()
add_subdirectory(third_party/googletest EXCLUDE_FROM_ALL)
add_subdirectory(tests)
