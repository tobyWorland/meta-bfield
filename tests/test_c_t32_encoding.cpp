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

TEST(t32_encoding_spec, bkpt_t1) {
    constexpr uint16_t bkpt_t1_42 = 0xBE2A; // BKPT 42

    uint16_t out;
    bkpt_t1_parts parts_for_enc{};
    bkpt_t1_parts parts_for_dec{};

    EXPECT_TRUE(match_bkpt_t1(bkpt_t1_42));

    parts_for_enc.imm8 = 42;

    EXPECT_EQ(encode_bkpt_t1(&out, &parts_for_enc), 16);
    EXPECT_EQ(out, bkpt_t1_42);

    parts_for_dec = decode_bkpt_t1(out);
    EXPECT_EQ(std::memcmp(&parts_for_dec, &parts_for_enc, sizeof(parts_for_dec)), 0);
}

TEST(t32_encoding_spec, ldr_i_t1) {
    constexpr uint16_t ldr_i_t1_r4_r2_4 = 0x6854; // LDR r4, [r2, 4]

    uint16_t out;
    ldr_i_t1_parts parts_for_enc{};
    ldr_i_t1_parts parts_for_dec{};

    EXPECT_TRUE(match_ldr_i_t1(ldr_i_t1_r4_r2_4));

    parts_for_enc.Rt = 4;
    parts_for_enc.Rn = 2;
    parts_for_enc.imm7 = 4;

    EXPECT_EQ(encode_ldr_i_t1(&out, &parts_for_enc), 16);
    EXPECT_EQ(out, ldr_i_t1_r4_r2_4);

    parts_for_dec = decode_ldr_i_t1(out);
    EXPECT_EQ(std::memcmp(&parts_for_dec, &parts_for_enc, sizeof(parts_for_dec)), 0);
}

TEST(t32_encoding_spec, ldr_i_t2_sponly) {
    constexpr uint16_t ldr_i_t2_sponly_r4_sp_4 = 0x9C01; // LDR r4, [SP, 4]

    uint16_t out;
    ldr_i_t2_sponly_parts parts_for_enc{};
    ldr_i_t2_sponly_parts parts_for_dec{};

    EXPECT_TRUE(match_ldr_i_t2_sponly(ldr_i_t2_sponly_r4_sp_4));

    parts_for_enc.Rt = 4;
    parts_for_enc.imm10 = 4;

    EXPECT_EQ(encode_ldr_i_t2_sponly(&out, &parts_for_enc), 16);
    EXPECT_EQ(out, ldr_i_t2_sponly_r4_sp_4);

    parts_for_dec = decode_ldr_i_t2_sponly(out);
    EXPECT_EQ(std::memcmp(&parts_for_dec, &parts_for_enc, sizeof(parts_for_dec)), 0);
}

TEST(t32_encoding_spec, ldr_i_t3) {
    constexpr uint32_t ldr_i_t3_r7_r8_4 = 0x7004'F8D8; // LDR.W r7, [r8, 4]

    uint32_t out;
    ldr_i_t3_parts parts_for_enc{};
    ldr_i_t3_parts parts_for_dec{};

    EXPECT_TRUE(match_ldr_i_t3(ldr_i_t3_r7_r8_4));

    parts_for_enc.Rt = 7;
    parts_for_enc.Rn = 8;
    parts_for_enc.imm12 = 4;

    EXPECT_EQ(encode_ldr_i_t3(&out, &parts_for_enc), 32);
    EXPECT_EQ(out, ldr_i_t3_r7_r8_4);

    parts_for_dec = decode_ldr_i_t3(out);
    EXPECT_EQ(std::memcmp(&parts_for_dec, &parts_for_enc, sizeof(parts_for_dec)), 0);
}

TEST(t32_encoding_spec, ldr_i_t4_negoff) {
    constexpr uint32_t ldr_i_t4_r7_r8_neg4 = 0x7C04'F858; // LDR.W r7, [r8, -4]

    uint32_t out;
    ldr_i_t4_parts parts_for_enc{};
    ldr_i_t4_parts parts_for_dec{};

    EXPECT_TRUE(match_ldr_i_t4(ldr_i_t4_r7_r8_neg4));

    parts_for_enc.Rt = 7;
    parts_for_enc.Rn = 8;
    parts_for_enc.imm8 = 4;
    parts_for_enc.index = 1;
    parts_for_enc.add = 0;
    parts_for_enc.writeback = 0;

    EXPECT_EQ(encode_ldr_i_t4(&out, &parts_for_enc), 32);
    EXPECT_EQ(out, ldr_i_t4_r7_r8_neg4);

    parts_for_dec = decode_ldr_i_t4(out);
    EXPECT_EQ(std::memcmp(&parts_for_dec, &parts_for_enc, sizeof(parts_for_dec)), 0);
}

TEST(t32_encoding_spec, ldr_i_t4_preindex) {
    constexpr uint32_t ldr_i_t4_r7_r8_neg4 = 0x7F04'F858; // LDR.W r7, [r8, 4]!

    uint32_t out;
    ldr_i_t4_parts parts_for_enc{};
    ldr_i_t4_parts parts_for_dec{};

    EXPECT_TRUE(match_ldr_i_t4(ldr_i_t4_r7_r8_neg4));

    parts_for_enc.Rt = 7;
    parts_for_enc.Rn = 8;
    parts_for_enc.imm8 = 4;
    parts_for_enc.index = 1;
    parts_for_enc.add = 1;
    parts_for_enc.writeback = 1;

    EXPECT_EQ(encode_ldr_i_t4(&out, &parts_for_enc), 32);
    EXPECT_EQ(out, ldr_i_t4_r7_r8_neg4);

    parts_for_dec = decode_ldr_i_t4(out);
    EXPECT_EQ(std::memcmp(&parts_for_dec, &parts_for_enc, sizeof(parts_for_dec)), 0);
}

TEST(t32_encoding_spec, ldr_i_t4_postindex) {
    constexpr uint32_t ldr_i_t4_r7_r8_neg4 = 0x7B04'F858; // LDR.W r7, [r8], 4

    uint32_t out;
    ldr_i_t4_parts parts_for_enc{};
    ldr_i_t4_parts parts_for_dec{};

    EXPECT_TRUE(match_ldr_i_t4(ldr_i_t4_r7_r8_neg4));

    parts_for_enc.Rt = 7;
    parts_for_enc.Rn = 8;
    parts_for_enc.imm8 = 4;
    parts_for_enc.index = 0;
    parts_for_enc.add = 1;
    parts_for_enc.writeback = 1;

    EXPECT_EQ(encode_ldr_i_t4(&out, &parts_for_enc), 32);
    EXPECT_EQ(out, ldr_i_t4_r7_r8_neg4);

    parts_for_dec = decode_ldr_i_t4(out);
    EXPECT_EQ(std::memcmp(&parts_for_dec, &parts_for_enc, sizeof(parts_for_dec)), 0);
}

TEST(t32_encoding_spec, ldr_lit_t1) {
    constexpr uint16_t ldr_lit_t1_r2_pc_4 = 0x4A01; // LDR r2, [pc, 4]

    uint16_t out;
    ldr_lit_t1_parts parts_for_enc{};
    ldr_lit_t1_parts parts_for_dec{};

    EXPECT_TRUE(match_ldr_lit_t1(ldr_lit_t1_r2_pc_4));

    parts_for_enc.Rt = 2;
    parts_for_enc.imm10 = 4;

    EXPECT_EQ(encode_ldr_lit_t1(&out, &parts_for_enc), 16);
    EXPECT_EQ(out, ldr_lit_t1_r2_pc_4);

    parts_for_dec = decode_ldr_lit_t1(out);
    EXPECT_EQ(std::memcmp(&parts_for_dec, &parts_for_enc, sizeof(parts_for_dec)), 0);
}

TEST(t32_encoding_spec, ldr_lit_t2_add) {
    constexpr uint32_t ldr_lit_t2_r8_1 = 0x8001'F8DF; // LDR.W r8, [pc, 1]

    uint32_t out;
    ldr_lit_t2_parts parts_for_enc{};
    ldr_lit_t2_parts parts_for_dec{};

    EXPECT_TRUE(match_ldr_lit_t2(ldr_lit_t2_r8_1));

    parts_for_enc.Rt = 8;
    parts_for_enc.add = 1;
    parts_for_enc.imm12 = 1;

    EXPECT_EQ(encode_ldr_lit_t2(&out, &parts_for_enc), 32);
    EXPECT_EQ(out, ldr_lit_t2_r8_1);

    parts_for_dec = decode_ldr_lit_t2(out);
    EXPECT_EQ(std::memcmp(&parts_for_dec, &parts_for_enc, sizeof(parts_for_dec)), 0);
}

TEST(t32_encoding_spec, ldr_lit_t2_sub) {
    constexpr uint32_t ldr_lit_t2_r8_1 = 0x8001'F85F; // LDR.W r8, [pc, -1]

    uint32_t out;
    ldr_lit_t2_parts parts_for_enc{};
    ldr_lit_t2_parts parts_for_dec{};

    EXPECT_TRUE(match_ldr_lit_t2(ldr_lit_t2_r8_1));

    parts_for_enc.Rt = 8;
    parts_for_enc.add = 0;
    parts_for_enc.imm12 = 1;

    EXPECT_EQ(encode_ldr_lit_t2(&out, &parts_for_enc), 32);
    EXPECT_EQ(out, ldr_lit_t2_r8_1);

    parts_for_dec = decode_ldr_lit_t2(out);
    EXPECT_EQ(std::memcmp(&parts_for_dec, &parts_for_enc, sizeof(parts_for_dec)), 0);
}

TEST(t32_encoding_spec, ldr_r_t1) {
    constexpr uint16_t ldr_r_t1_r1_r2_r3 = 0x58d1; // LDR r1, [r2, r3]

    uint16_t out;
    ldr_r_t1_parts parts_for_enc{};
    ldr_r_t1_parts parts_for_dec{};

    EXPECT_TRUE(match_ldr_r_t1(ldr_r_t1_r1_r2_r3));

    parts_for_enc.Rt = 1;
    parts_for_enc.Rn = 2;
    parts_for_enc.Rm = 3;

    EXPECT_EQ(encode_ldr_r_t1(&out, &parts_for_enc), 16);
    EXPECT_EQ(out, ldr_r_t1_r1_r2_r3);

    parts_for_dec = decode_ldr_r_t1(out);
    EXPECT_EQ(std::memcmp(&parts_for_dec, &parts_for_enc, sizeof(parts_for_dec)), 0);
}

TEST(t32_encoding_spec, ldr_r_t2_lsl) {
    constexpr uint32_t ldr_r_t2_r1_r2_r3_lsl2 = 0x1023'F852; // LDR.W r1, [r2, r3, LSL 2]

    uint32_t out;
    ldr_r_t2_lsl_parts parts_for_enc{};
    ldr_r_t2_lsl_parts parts_for_dec{};

    EXPECT_TRUE(match_ldr_r_t2_lsl(ldr_r_t2_r1_r2_r3_lsl2));

    parts_for_enc.Rt = 1;
    parts_for_enc.Rn = 2;
    parts_for_enc.Rm = 3;
    parts_for_enc.lsl_shift_imm2 = 2;

    EXPECT_EQ(encode_ldr_r_t2_lsl(&out, &parts_for_enc), 32);
    EXPECT_EQ(out, ldr_r_t2_r1_r2_r3_lsl2);

    parts_for_dec = decode_ldr_r_t2_lsl(out);
    EXPECT_EQ(std::memcmp(&parts_for_dec, &parts_for_enc, sizeof(parts_for_dec)), 0);
}

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

TEST(t32_encoding_spec, udf_t1) {
    constexpr uint16_t udf_t1_21 = 0xDE15; // UDF 21

    uint16_t out;
    udf_t1_parts parts_for_enc{};
    udf_t1_parts parts_for_dec{};

    EXPECT_TRUE(match_udf_t1(udf_t1_21));

    parts_for_enc.imm8 = 21;

    EXPECT_EQ(encode_udf_t1(&out, &parts_for_enc), 16);
    EXPECT_EQ(out, udf_t1_21);

    parts_for_dec = decode_udf_t1(out);
    EXPECT_EQ(std::memcmp(&parts_for_dec, &parts_for_enc, sizeof(parts_for_dec)), 0);
}

TEST(t32_encoding_spec, udf_t2) {
    constexpr uint32_t udf_t2_1234h = 0xA234'F7F1; // UDF.W 0x1234

    uint32_t out;
    udf_t2_parts parts_for_enc{};
    udf_t2_parts parts_for_dec{};

    EXPECT_TRUE(match_udf_t2(udf_t2_1234h));

    parts_for_enc.imm16 = 0x1234;

    EXPECT_EQ(encode_udf_t2(&out, &parts_for_enc), 32);
    EXPECT_EQ(out, udf_t2_1234h);

    parts_for_dec = decode_udf_t2(out);
    EXPECT_EQ(std::memcmp(&parts_for_dec, &parts_for_enc, sizeof(parts_for_dec)), 0);
}
