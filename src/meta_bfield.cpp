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

int main(int argc, char **argv) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <spec> <outpath>\n";
        return 1;
    }

    std::string input_spec_path = argv[1];
    std::string output_path = argv[2];

    SpecReader spec_reader;
    auto fields = spec_reader.read_from_spec(argv[1]);

#if 0
    for (const auto &field: fields) {
        dump_field(field);
    }
#endif

    if (generate_fields(output_path, fields)) {
        std::cout << "Generate successful\n";
    } else {
        std::cout << "Generate failed\n";
    }

    return 0;
}
