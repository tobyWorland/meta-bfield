#include <iostream>

#include "bfield_builder.hpp"

int main() {
    BFieldBuilder builder;

    builder.set_field_name("bkpt_t1");
    builder.set_field_width(16);
    builder.push_back_part(BPart(8, 0b10111110));
    builder.push_back_part(BPart("imm8", 8));
    BField bkpt_t1 = builder.build();

    std::cout << bkpt_t1.name() << " " << bkpt_t1.width() << '\n';
    for (auto &part : bkpt_t1.parts()) {
        std::cout << part.name() << " " << part.width();
        if (part.is_reserved()) {
            std::cout << " 0x" << std::hex << part.reserved_value();
        }
        std::cout << '\n';
    }

    return 0;
}
