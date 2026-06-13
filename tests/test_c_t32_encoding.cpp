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

TEST(t32_encoding_spec, bl_t1) {
    constexpr uint32_t bl_t1_pcoff4     = 0xF802'F000;
    constexpr uint32_t bl_t1_pcoff_neg4 = 0xFFFE'F7FF;

    uint32_t out;
    bl_t1_parts parts_for_enc{};
    bl_t1_parts parts_for_dec{};

    EXPECT_TRUE(match_bl_t1(bl_t1_pcoff4));
    EXPECT_TRUE(match_bl_t1(bl_t1_pcoff_neg4));

    parts_for_enc.simm25 = 4;

    EXPECT_EQ(encode_bl_t1(&out, &parts_for_enc), 32);
    EXPECT_EQ(out, bl_t1_pcoff4);

    parts_for_dec = decode_bl_t1(out);
    EXPECT_EQ(std::memcmp(&parts_for_dec, &parts_for_enc, sizeof(parts_for_dec)), 0);

    parts_for_enc.simm25 = -4;

    EXPECT_EQ(encode_bl_t1(&out, &parts_for_enc), 32);
    EXPECT_EQ(out, bl_t1_pcoff_neg4);

    parts_for_dec = decode_bl_t1(out);
    EXPECT_EQ(std::memcmp(&parts_for_dec, &parts_for_enc, sizeof(parts_for_dec)), 0);
}

TEST(t32_encoding_spec, blx_t1) {
    constexpr uint16_t blx_r4 = 0x47A0; // BLX R4

    uint16_t out;
    blx_t1_parts parts_for_enc{};
    blx_t1_parts parts_for_dec{};

    EXPECT_TRUE(match_blx_t1(blx_r4));

    parts_for_enc.Rm = 4; // LR

    EXPECT_EQ(encode_blx_t1(&out, &parts_for_enc), 16);
    EXPECT_EQ(out, blx_r4);

    parts_for_dec = decode_blx_t1(out);
    EXPECT_EQ(std::memcmp(&parts_for_dec, &parts_for_enc, sizeof(parts_for_dec)), 0);
}

TEST(t32_encoding_spec, bx_t1) {
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

TEST(t32_encoding_spec, pop_t1) {
    constexpr uint16_t pop_t1_2_4_pc = 0xBD14; // POP {r2, r4, PC}

    uint16_t out;
    pop_t1_parts parts_for_enc{};
    pop_t1_parts parts_for_dec{};

    EXPECT_TRUE(match_pop_t1(pop_t1_2_4_pc));

    parts_for_enc.regs = (1 << 2) | (1 << 4);
    parts_for_enc.pc = 1;

    EXPECT_EQ(encode_pop_t1(&out, &parts_for_enc), 16);
    EXPECT_EQ(out, pop_t1_2_4_pc);

    parts_for_dec = decode_pop_t1(out);
    EXPECT_EQ(std::memcmp(&parts_for_dec, &parts_for_enc, sizeof(parts_for_dec)), 0);
}

TEST(t32_encoding_spec, pop_t2) {
    constexpr uint32_t pop_t2_pc_lr_10_6_3_1 = 0xC44A'E8BD; // POP {r1, r3, r6, r10, LR, PC}

    uint32_t out;
    pop_t2_parts parts_for_enc{};
    pop_t2_parts parts_for_dec{};

    EXPECT_TRUE(match_pop_t2(pop_t2_pc_lr_10_6_3_1));

    parts_for_enc.regs = (1 << 10) | (1 << 6) | (1 << 3) | (1 << 1);
    parts_for_enc.pc = 1;
    parts_for_enc.lr = 1;

    EXPECT_EQ(encode_pop_t2(&out, &parts_for_enc), 32);
    EXPECT_EQ(out, pop_t2_pc_lr_10_6_3_1);

    parts_for_dec = decode_pop_t2(out);
    EXPECT_EQ(std::memcmp(&parts_for_dec, &parts_for_enc, sizeof(parts_for_dec)), 0);
}

TEST(t32_encoding_spec, pop_t3) {
    constexpr uint32_t pop_t3_r9 = 0x9B04'F85D; // POP {R9}

    uint32_t out;
    pop_t3_parts parts_for_enc{};
    pop_t3_parts parts_for_dec{};

    EXPECT_TRUE(match_pop_t3(pop_t3_r9));

    parts_for_enc.Rt = 9;

    EXPECT_EQ(encode_pop_t3(&out, &parts_for_enc), 32);
    EXPECT_EQ(out, pop_t3_r9);

    parts_for_dec = decode_pop_t3(out);
    EXPECT_EQ(std::memcmp(&parts_for_dec, &parts_for_enc, sizeof(parts_for_dec)), 0);
}

TEST(t32_encoding_spec, push_t1) {
    constexpr uint16_t push_t1_2_4_lr = 0xB514; // PUSH {r2, r4, PC}

    uint16_t out;
    push_t1_parts parts_for_enc{};
    push_t1_parts parts_for_dec{};

    EXPECT_TRUE(match_push_t1(push_t1_2_4_lr));

    parts_for_enc.regs = (1 << 2) | (1 << 4);
    parts_for_enc.lr = 1;

    EXPECT_EQ(encode_push_t1(&out, &parts_for_enc), 16);
    EXPECT_EQ(out, push_t1_2_4_lr);

    parts_for_dec = decode_push_t1(out);
    EXPECT_EQ(std::memcmp(&parts_for_dec, &parts_for_enc, sizeof(parts_for_dec)), 0);
}

TEST(t32_encoding_spec, push_t2) {
    constexpr uint32_t push_t2_lr_10_6_3_1 = 0x444A'E92D; // PUSH {r1, r3, r6, r10, LR}

    uint32_t out;
    push_t2_parts parts_for_enc{};
    push_t2_parts parts_for_dec{};

    EXPECT_TRUE(match_push_t2(push_t2_lr_10_6_3_1));

    parts_for_enc.regs = (1 << 10) | (1 << 6) | (1 << 3) | (1 << 1);
    parts_for_enc.lr = 1;

    EXPECT_EQ(encode_push_t2(&out, &parts_for_enc), 32);
    EXPECT_EQ(out, push_t2_lr_10_6_3_1);

    parts_for_dec = decode_push_t2(out);
    EXPECT_EQ(std::memcmp(&parts_for_dec, &parts_for_enc, sizeof(parts_for_dec)), 0);
}

TEST(t32_encoding_spec, push_t3) {
    constexpr uint32_t push_t3_r9 = 0x9D04'F84D; // PUSH {R9}

    uint32_t out;
    push_t3_parts parts_for_enc{};
    push_t3_parts parts_for_dec{};

    EXPECT_TRUE(match_push_t3(push_t3_r9));

    parts_for_enc.Rt = 9;

    EXPECT_EQ(encode_push_t3(&out, &parts_for_enc), 32);
    EXPECT_EQ(out, push_t3_r9);

    parts_for_dec = decode_push_t3(out);
    EXPECT_EQ(std::memcmp(&parts_for_dec, &parts_for_enc, sizeof(parts_for_dec)), 0);
}

// TODO: SCV
// TODO: UDF
