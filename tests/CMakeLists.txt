
add_executable(test_bfield test_bfield.cpp)
target_link_libraries(test_bfield GTest::gtest_main bfield)

add_executable(test_c_prelude test_c_prelude.cpp)
target_link_libraries(test_c_prelude GTest::gtest_main)
target_include_directories(test_c_prelude PRIVATE "${CMAKE_SOURCE_DIR}/preludes")

include(GoogleTest)
gtest_discover_tests(test_bfield)
gtest_discover_tests(test_c_prelude)
