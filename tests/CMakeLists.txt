
add_executable(test_bfield test_bfield.cpp)
target_link_libraries(test_bfield GTest::gtest_main bfield)

include(GoogleTest)
gtest_discover_tests(test_bfield)
