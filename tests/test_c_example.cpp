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

    uint16_t y = 0;
    two_int8s_parts.sa = -2;
    two_int8s_parts.sb = 4;
    EXPECT_EQ(encode_two_int8s(&y, &two_int8s_parts), 16);
    EXPECT_EQ(y, 0b1111'1110'0000'0100);
}

TEST(example_sec, onetwothree) {
    onetwothree_parts ott_parts;

    ott_parts.one = 1;
    ott_parts.two = 2;
    ott_parts.three = 3;

    uint16_t expected = (2 << 8) | (4 << 4) | 6;
    uint16_t actual;

    EXPECT_EQ(encode_onetwothree(&actual, &ott_parts), 12);
    EXPECT_EQ(actual, expected);

    ott_parts = decode_onetwothree(0);
    EXPECT_EQ(ott_parts.one, 0b1111);
    EXPECT_EQ(ott_parts.two, 0b1110);
    EXPECT_EQ(ott_parts.three, 0b1101);
}
