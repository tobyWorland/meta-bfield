#include <iostream>
#include <string>

#include "c_generator.hpp"
#include "bfield.hpp"
#include "spec.hpp"

static void dump_field(const BField &field) {
    std::cout << field.name() << " " << field.width() << '\n';
    for (auto &part : field.parts()) {
        std::cout << part->name() << " " << part->width();
        if (part->is_reserved()) {
            std::cout << " 0x" << std::hex << part->reserved_value();
        }
        std::cout << '\n';
    }
}

int main() {
    SpecReader spec_reader;
    auto fields = spec_reader.read_from_spec("example_specs/t32.json");

#if 0
    for (const auto &field: fields) {
        dump_field(field);
    }
#endif

    std::string output_path = "build/out/t32_encoding";
    if (generate_fields(output_path, fields)) {
        std::cout << "Generate successful\n";
    } else {
        std::cout << "Generate failed\n";
    }

    return 0;
}
