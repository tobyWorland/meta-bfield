#include <gtest/gtest.h>

#include <cstdint>

extern "C" {
#include "example.h"
}

TEST(example_spec, two_int8s) {
    uint16_t x = ~0;

    two_int8s_parts two_int8s_parts = decode_two_int8s(~0);
    EXPECT_EQ(two_int8s_parts.sa, -1);
    EXPECT_EQ(two_int8s_parts.sb, -1);
}
