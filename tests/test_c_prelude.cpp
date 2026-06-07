#include <gtest/gtest.h>

#include "prelude.c"

#include <cstdint>
#include <unordered_map>

TEST(c_prelude, bit) {
	uint32_t x = 1;
	for (unsigned i = 0; i < 32; i++, x <<= 1) {
		EXPECT_EQ(BIT(i), x);
	}
}

TEST(c_prelude, bit_mask) {
	EXPECT_EQ(BIT_MASK(32), 0xFFFFFFFF);
	EXPECT_EQ(BIT_MASK(0),  0x0);
	EXPECT_EQ(BIT_MASK(3),  0x7);
	EXPECT_EQ(BIT_MASK(4),  0xF);
	EXPECT_EQ(BIT_MASK(5),  0x1F);
}

TEST(c_prelude, bit_extract) {
	uint32_t x = 0xDEADBEEF;
	EXPECT_EQ(BIT_EXTRACT(x, 4, 8),    0xEE);
	EXPECT_EQ(BIT_EXTRACT(x, 4, 16), 0xDBEE);

	EXPECT_EQ(BIT_EXTRACT(0b11010111, 3, 4), 0b1010);
}

TEST(c_prelude, bit_unsigned_check_fit) {
	EXPECT_EQ(UNSIGNED_CHECK_FIT(0b0000, 3), true);
	EXPECT_EQ(UNSIGNED_CHECK_FIT(0b0001, 3), true);
	EXPECT_EQ(UNSIGNED_CHECK_FIT(0b0010, 3), true);
	EXPECT_EQ(UNSIGNED_CHECK_FIT(0b0011, 3), true);
	EXPECT_EQ(UNSIGNED_CHECK_FIT(0b0100, 3), true);
	EXPECT_EQ(UNSIGNED_CHECK_FIT(0b0101, 3), true);
	EXPECT_EQ(UNSIGNED_CHECK_FIT(0b0110, 3), true);
	EXPECT_EQ(UNSIGNED_CHECK_FIT(0b0111, 3), true);
	EXPECT_EQ(UNSIGNED_CHECK_FIT(0b1000, 3), false);
	EXPECT_EQ(UNSIGNED_CHECK_FIT(0b1001, 3), false);
	EXPECT_EQ(UNSIGNED_CHECK_FIT(0b1010, 3), false);
	EXPECT_EQ(UNSIGNED_CHECK_FIT(0b1011, 3), false);
}

TEST(c_prelude, bit_signed_check_fit) {
	EXPECT_EQ(SIGNED_CHECK_FIT(-0b0111, 3), false);
	EXPECT_EQ(SIGNED_CHECK_FIT(-0b0110, 3), false);
	EXPECT_EQ(SIGNED_CHECK_FIT(-0b0101, 3), false);
	EXPECT_EQ(SIGNED_CHECK_FIT(-0b0100, 3), true);
	EXPECT_EQ(SIGNED_CHECK_FIT(-0b0011, 3), true);
	EXPECT_EQ(SIGNED_CHECK_FIT(-0b0010, 3), true);
	EXPECT_EQ(SIGNED_CHECK_FIT(-0b0001, 3), true);
	EXPECT_EQ(SIGNED_CHECK_FIT( 0b0000, 3), true);
	EXPECT_EQ(SIGNED_CHECK_FIT( 0b0001, 3), true);
	EXPECT_EQ(SIGNED_CHECK_FIT( 0b0010, 3), true);
	EXPECT_EQ(SIGNED_CHECK_FIT( 0b0011, 3), true);
	EXPECT_EQ(SIGNED_CHECK_FIT( 0b0100, 3), false);
	EXPECT_EQ(SIGNED_CHECK_FIT( 0b0101, 3), false);
	EXPECT_EQ(SIGNED_CHECK_FIT( 0b0110, 3), false);
	EXPECT_EQ(SIGNED_CHECK_FIT( 0b0111, 3), false);
}

static const std::unordered_map<int, int> imm3map {
	{ -4, 0b100 },
	{ -3, 0b101 },
	{ -2, 0b110 },
	{ -1, 0b111 },
	{  0, 0b000 },
	{  1, 0b001 },
	{  2, 0b010 },
	{  3, 0b011 },
};

TEST(c_prelude, bit_unsigned_shrink) {
	for (const auto &pair : imm3map) {
		if (pair.first >= 0) {
			EXPECT_EQ(UNSIGNED_SHRINK_TO(pair.first, 3), pair.second);
		}
	}
}

TEST(c_prelude, bit_signed_shrink) {
	for (const auto &pair : imm3map) {
		EXPECT_EQ(SIGNED_SHRINK_TO(pair.first, 3), pair.second);
	}
}
