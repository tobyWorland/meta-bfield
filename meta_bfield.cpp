#include <iostream>
#include <string>

#include "c_generator.hpp"
#include "bfield.hpp"
#include "bfield_builder.hpp"

static void dump_field(const BField &field) {
    std::cout << field.name() << " " << field.width() << '\n';
    for (auto &part : field.parts()) {
        std::cout << part.name() << " " << part.width();
        if (part.is_reserved()) {
            std::cout << " 0x" << std::hex << part.reserved_value();
        }
        std::cout << '\n';
    }
}

int main() {
    BFieldBuilder builder;
    std::vector<BField> fields;

    builder.set_field_name("bkpt_t1");
    builder.set_field_width(16);
    builder.push_back_part(res_from_binstring("1011 1110"));
    builder.push_back_part(BPart("imm8", 8));
    fields.push_back(builder.build());

    for (const auto &field: fields) {
        dump_field(field);
    }

    std::string output_path = "build/out/field";
    if (generate_fields(output_path, fields)) {
        std::cout << "Generate successful\n";
    } else {
        std::cout << "Generate failed\n";
    }

    return 0;
}
