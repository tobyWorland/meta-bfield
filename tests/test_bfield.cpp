#include <gtest/gtest.h>
#include "../bfield.hpp"
#include "../bfield_builder.hpp"

TEST(bfield, bpart_reserved) {
    EXPECT_NO_THROW(BPart(4, 5));
    EXPECT_THROW(BPart(2, 5), std::invalid_argument);
}

TEST(bfield, bfield_parts_fall_short) {
    std::vector<BPart> parts{BPart(8, 0)};
    EXPECT_THROW(BField("short", 16, parts), std::invalid_argument);
}

TEST(bfield, bfield_parts_too_long) {
    std::vector<BPart> parts{BPart(8, 0)};
    EXPECT_THROW(BField("long", 4, parts), std::invalid_argument);
}

TEST(bfield, bfield_success) {
    std::vector<BPart> parts {
        BPart(8, 0),
        BPart(8, 0)
    };
    EXPECT_NO_THROW(BField("pair", 16, parts));
}

TEST(bfieldbuilder, missing_field_properties) {
    BFieldBuilder builder;

    builder.set_field_width(2);
    EXPECT_THROW(builder.build(), BFieldBuilderError);

    builder.reset();

    builder.set_field_name("name");
    EXPECT_THROW(builder.build(), BFieldBuilderError);
}

TEST(bfieldbuilder, missing_parts) {
    BFieldBuilder builder;

    builder.set_field_name("pair");
    builder.set_field_width(16);
    EXPECT_THROW(builder.build(), std::invalid_argument);
}

TEST(bfieldbuilder, success) {
    BFieldBuilder builder;

    builder.set_field_name("pair");
    builder.set_field_width(16);
    builder.push_back_part(BPart(8, 0));
    builder.push_back_part(BPart(8, 0));
    EXPECT_NO_THROW(builder.build());
}

TEST(res_from_binstring, simple) {
    auto rval = res_from_binstring("");
    EXPECT_EQ(rval.width, 0);
    EXPECT_EQ(rval.value, 0);

    rval = res_from_binstring("101");
    EXPECT_EQ(rval.width, 3);
    EXPECT_EQ(rval.value, 0b101);

    rval = res_from_binstring("11 1 1");
    EXPECT_EQ(rval.width, 4);
    EXPECT_EQ(rval.value, 0b1111);

    EXPECT_THROW(res_from_binstring("11 1 1 1a0"), std::invalid_argument);

    // TODO: Test for overflow
}
