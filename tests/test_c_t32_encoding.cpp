#include <gtest/gtest.h>

#include <cstdint>

extern "C" {
#include "t32_encoding.h"
}

TEST(t32_encoding_spec, b) {
    // TODO: ...
}

TEST(t32_encoding_spec, bxlr) {
    constexpr uint16_t bxlr = 0x4770; // BX LR
    uint16_t out;
    bx_t1_parts parts;

    parts.Rm = 14; // LR

    EXPECT_EQ(encode_bx_t1(&out, &parts), 16);
    EXPECT_EQ(out, bxlr);

    parts = {};

    parts = decode_bx_t1(bxlr);
    EXPECT_EQ(parts.Rm, 14);
}

// TODO: BKPT
// TODO: NOP
// TODO: NOP.W
// TODO: MOV_i
// TODO: MOV_r
// TODO: MOVW
// TODO: SCV
// TODO: UDF
