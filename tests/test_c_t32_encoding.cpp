#include <gtest/gtest.h>

#include <cstdint>
#include <cstring>

extern "C" {
#include "t32_encoding.h"
}

enum thumb_condition {
    TC_EQ = 0,
    TC_NE = 1,
    TC_CS = 2,
    TC_CC = 3,
    TC_MI = 4,
    TC_PL = 5,
    TC_VS = 6,
    TC_VC = 7,
    TC_HI = 8,
    TC_LS = 9,
    TC_GE = 10,
    TC_LT = 11,
    TC_GT = 12,
    TC_LE = 13,
    TC_AL = 14
};

// NOTE: PC offsets will be from the program counter +4 because of the
// way the hardware behaves. For example to loop forever `B .` it
// would have a immediate of -4 to branch to the current
// instruction. (regardless of if it's narrow or wide)

TEST(t32_encoding_spec, b_t1) {
    constexpr uint16_t bt1_eq_pcoff4     = 0xD002;
    constexpr uint16_t bt1_ne_pcoff_neg4 = 0xD1FE;

    uint16_t out;
    b_cond_t1_noit_parts parts_for_enc{};
    b_cond_t1_noit_parts parts_for_dec{};

    EXPECT_TRUE(match_b_cond_t1_noit(bt1_eq_pcoff4));
    EXPECT_TRUE(match_b_cond_t1_noit(bt1_ne_pcoff_neg4));

    parts_for_enc.cond = TC_EQ;
    parts_for_enc.simm8 = 4;

    EXPECT_EQ(encode_b_cond_t1_noit(&out, &parts_for_enc), 16);
    EXPECT_EQ(out, bt1_eq_pcoff4);

    parts_for_dec = decode_b_cond_t1_noit(out);
    EXPECT_EQ(std::memcmp(&parts_for_dec, &parts_for_enc, sizeof(parts_for_dec)), 0);

    parts_for_enc.cond = TC_NE;
    parts_for_enc.simm8 = -4;

    EXPECT_EQ(encode_b_cond_t1_noit(&out, &parts_for_enc), 16);
    EXPECT_EQ(out, bt1_ne_pcoff_neg4);

    parts_for_dec = decode_b_cond_t1_noit(out);
    EXPECT_EQ(std::memcmp(&parts_for_dec, &parts_for_enc, sizeof(parts_for_dec)), 0);
}

TEST(t32_encoding_spec, b_t2) {
    constexpr uint16_t bt2_pcoff4     = 0xE002;
    constexpr uint16_t bt2_pcoff_neg4 = 0xE7FE;

    uint16_t out;
    b_t2_parts parts_for_enc{};
    b_t2_parts parts_for_dec{};

    EXPECT_TRUE(match_b_t2(bt2_pcoff4));
    EXPECT_TRUE(match_b_t2(bt2_pcoff_neg4));

    parts_for_enc.simm11 = 4;

    EXPECT_EQ(encode_b_t2(&out, &parts_for_enc), 16);
    EXPECT_EQ(out, bt2_pcoff4);

    parts_for_dec = decode_b_t2(out);
    EXPECT_EQ(std::memcmp(&parts_for_dec, &parts_for_enc, sizeof(parts_for_dec)), 0);

    parts_for_enc.simm11 = -4;

    EXPECT_EQ(encode_b_t2(&out, &parts_for_enc), 16);
    EXPECT_EQ(out, bt2_pcoff_neg4);

    parts_for_dec = decode_b_t2(out);
    EXPECT_EQ(std::memcmp(&parts_for_dec, &parts_for_enc, sizeof(parts_for_dec)), 0);
}

TEST(t32_encoding_spec, b_t3) {
    constexpr uint32_t bt3_eq_pcoff4     = 0x8002'F000;
    constexpr uint32_t bt3_ne_pcoff_neg4 = 0xAFFE'F47F;

    uint32_t out;
    b_cond_t3_noit_parts parts_for_enc{};
    b_cond_t3_noit_parts parts_for_dec{};

    EXPECT_TRUE(match_b_cond_t3_noit(bt3_eq_pcoff4));
    EXPECT_TRUE(match_b_cond_t3_noit(bt3_ne_pcoff_neg4));

    parts_for_enc.cond = TC_EQ;
    parts_for_enc.simm21 = 4;

    EXPECT_EQ(encode_b_cond_t3_noit(&out, &parts_for_enc), 32);
    EXPECT_EQ(out, bt3_eq_pcoff4);

    parts_for_dec = decode_b_cond_t3_noit(out);
    EXPECT_EQ(std::memcmp(&parts_for_dec, &parts_for_enc, sizeof(parts_for_dec)), 0);

    parts_for_enc.cond = TC_NE;
    parts_for_enc.simm21 = -4;

    EXPECT_EQ(encode_b_cond_t3_noit(&out, &parts_for_enc), 32);
    EXPECT_EQ(out, bt3_ne_pcoff_neg4);

    parts_for_dec = decode_b_cond_t3_noit(out);
    EXPECT_EQ(std::memcmp(&parts_for_dec, &parts_for_enc, sizeof(parts_for_dec)), 0);
}

TEST(t32_encoding_spec, b_t4) {
    constexpr uint32_t bt4_pcoff4     = 0xB802'F000;
    constexpr uint32_t bt4_pcoff_neg4 = 0xBFFE'F7FF;

    uint32_t out;
    b_t4_parts parts_for_enc{};
    b_t4_parts parts_for_dec{};

    EXPECT_TRUE(match_b_t4(bt4_pcoff4));
    EXPECT_TRUE(match_b_t4(bt4_pcoff_neg4));

    parts_for_enc.simm25 = 4;

    EXPECT_EQ(encode_b_t4(&out, &parts_for_enc), 32);
    EXPECT_EQ(out, bt4_pcoff4);

    parts_for_dec = decode_b_t4(out);
    EXPECT_EQ(std::memcmp(&parts_for_dec, &parts_for_enc, sizeof(parts_for_dec)), 0);

    parts_for_enc.simm25 = -4;

    EXPECT_EQ(encode_b_t4(&out, &parts_for_enc), 32);
    EXPECT_EQ(out, bt4_pcoff_neg4);

    parts_for_dec = decode_b_t4(out);
    EXPECT_EQ(std::memcmp(&parts_for_dec, &parts_for_enc, sizeof(parts_for_dec)), 0);
}

TEST(t32_encoding_spec, bxlr) {
    constexpr uint16_t bxlr = 0x4770; // BX LR

    uint16_t out;
    bx_t1_parts parts_for_enc{};
    bx_t1_parts parts_for_dec{};

    EXPECT_TRUE(match_bx_t1(bxlr));

    parts_for_enc.Rm = 14; // LR

    EXPECT_EQ(encode_bx_t1(&out, &parts_for_enc), 16);
    EXPECT_EQ(out, bxlr);

    parts_for_dec = decode_bx_t1(out);
    EXPECT_EQ(std::memcmp(&parts_for_dec, &parts_for_enc, sizeof(parts_for_dec)), 0);
}

// TODO: BKPT
// TODO: NOP
// TODO: NOP.W
// TODO: MOV_i
// TODO: MOV_r
// TODO: MOVW
// TODO: SCV
// TODO: UDF
