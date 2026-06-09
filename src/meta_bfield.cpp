#include <iostream>
#include <string>

#include "c_generator.hpp"
#include "bfield.hpp"
#include "spec.hpp"

int main(int argc, char **argv) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <spec> <outpath>\n";
        return 1;
    }

    std::string input_spec_path = argv[1];
    std::string output_path = argv[2];

    SpecReader spec_reader;
    auto fields = spec_reader.read_from_spec(argv[1]);

    if (generate_fields(output_path, fields)) {
        std::cout << "Generate successful\n";
    } else {
        std::cout << "Generate failed\n";
    }

    return 0;
}
