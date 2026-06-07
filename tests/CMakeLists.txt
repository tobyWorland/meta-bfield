
add_executable(test_bfield test_bfield.cpp)
target_link_libraries(test_bfield GTest::gtest_main bfield)

add_executable(test_c_prelude test_c_prelude.cpp)
target_link_libraries(test_c_prelude GTest::gtest_main)
target_include_directories(test_c_prelude PRIVATE "${CMAKE_SOURCE_DIR}/preludes")

add_executable(test_c_example test_c_example.cpp "${CMAKE_BINARY_DIR}/example.c")
target_link_libraries(test_c_example GTest::gtest_main gen_example)

include(GoogleTest)
gtest_discover_tests(test_bfield)
gtest_discover_tests(test_c_prelude)
