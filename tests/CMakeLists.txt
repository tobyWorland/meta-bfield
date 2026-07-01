
add_executable(test_bfield test_bfield.cpp)
target_link_libraries(test_bfield GTest::gtest_main bfield)

add_executable(test_c_prelude test_c_prelude.cpp)
target_link_libraries(test_c_prelude GTest::gtest_main)
target_include_directories(test_c_prelude PRIVATE "${CMAKE_SOURCE_DIR}/preludes")

add_executable(test_c_example test_c_example.cpp)
target_link_libraries(test_c_example GTest::gtest_main gen_example)

add_executable(test_c_t32_encoding test_c_t32_encoding.cpp)
target_link_libraries(test_c_t32_encoding GTest::gtest_main gen_t32_encoding)

include(GoogleTest)
gtest_discover_tests(
    test_bfield
    XML_OUTPUT_DIR "${CMAKE_BINARY_DIR}/testresults"
)
gtest_discover_tests(
    test_c_prelude
    XML_OUTPUT_DIR "${CMAKE_BINARY_DIR}/testresults"
)
gtest_discover_tests(
    test_c_example
    XML_OUTPUT_DIR "${CMAKE_BINARY_DIR}/testresults"
)
gtest_discover_tests(
    test_c_t32_encoding
    XML_OUTPUT_DIR "${CMAKE_BINARY_DIR}/testresults"
)
