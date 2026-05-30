#include "c_generator.hpp"

#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>

static std::string file_from_filepath(const std::filesystem::path path) {
    return path.filename();
}

void struct_from_field(std::fstream &file, const BField &field) {
    file << "struct " << field.name() << "_parts" << " {\n";
    for (const BPart &part : field.parts()) {
        if (part.is_reserved())
            continue;

        assert(part.width() <= 32);
        file << "    uint32_t " << part.name() << ";\n";
    }
    file << "};\n\n";
}

bool generate_fields(const std::string output_basepath, std::vector<BField> fields) {
    std::string source_path = output_basepath + ".c";
    std::string header_path = output_basepath + ".h";
    std::string header_file = file_from_filepath(header_path);

    // TODO: Check if exist and only overwrite if there is a comment at the top "// AUTOGEN - DO NOT EDIT"
    std::fstream source(source_path, std::ios_base::out);
    std::fstream header(header_path, std::ios_base::out);
    bool open_error{false};

    if (!source.is_open()) {
        std::cerr << "Failed to open " << source_path << '\n';
        open_error = true;
    }

    if (!header.is_open()) {
        std::cerr << "Failed to open " << header_path << '\n';
        open_error = true;
    }

    if (open_error) {
        return false;
    }

    header << "#pragma once\n\n";
    source << "#include \"" << header_file << "\"\n\n";

    for (const BField &field : fields) {
        struct_from_field(header, field);
    }

    return true;
}
