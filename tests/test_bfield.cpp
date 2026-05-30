#include <gtest/gtest.h>
#include "../bfield.hpp"

TEST(bfield, bpart_reserved) {
    EXPECT_NO_THROW(BPart(4, 5));
    EXPECT_THROW(BPart(2, 5), std::invalid_argument);
}
