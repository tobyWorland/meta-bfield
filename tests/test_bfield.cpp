#include <gtest/gtest.h>
#include "../bfield.hpp"
#include "../bfield_builder.hpp"

TEST(bfield, bpart_reserved) {
    EXPECT_NO_THROW(BPart(4, 5));
    EXPECT_THROW(BPart(2, 5), std::invalid_argument);
}

TEST(bfieldbuilder, missing_field_properties) {
    BFieldBuilder builder;

    builder.set_field_width(2);
    EXPECT_THROW(builder.build(), BFieldBuilderError);

    builder.reset();

    builder.set_field_name("name");
    EXPECT_THROW(builder.build(), BFieldBuilderError);
}
