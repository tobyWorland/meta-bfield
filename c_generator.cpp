#include "c_generator.hpp"

#include <cassert>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>

static std::string file_from_filepath(const std::filesystem::path path) {
    return path.filename();
}

const char *type_from_width(unsigned width) {
    assert(width <= 32);
    if (width <= 8) {
        return "uint8_t";
    } else if (width <= 16) {
        return "uint16_t";
    } else {
        return "uint32_t";
    }
}

std::string struct_name_from_field(const BField &field) {
    return std::format("struct {}_parts", field.name());
}

void struct_from_field(std::fstream &file, const BField &field) {
    file << "struct " << struct_name_from_field(field) << " {\n";
    for (const BPart &part : field.parts()) {
        if (part.is_reserved())
            continue;

        assert(part.width() <= 32);
        // TODO: handle if signed
        file << "    " << type_from_width(part.width()) << " " << part.name() << ";\n";
    }
    file << "};\n\n";
}

void prototype_match_from_field(std::fstream &file, const BField &field) {
    file << "bool match_" << field.name() << "(" << type_from_width(field.width()) << " field)";
}

void prototype_encode_from_field(std::fstream &file, const BField &field) {
    file << "unsigned encode_" << field.name();
    file << "(" << type_from_width(field.width()) << " *out";
    file << ", " << struct_name_from_field(field);
    file << ")";
}

void prototype_decode_from_field(std::fstream &file, const BField &field) {
    file << struct_name_from_field(field);
    file << " decode_" << field.name();
    file << "(" << type_from_width(field.width()) << " field)";
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

    for (const BField &field : fields) {
        prototype_match_from_field(header, field);
        header << ";\n";
    }

    header << "\n";

    for (const BField &field : fields) {
        prototype_encode_from_field(header, field);
        header << ";\n";
    }

    header << "\n";

    for (const BField &field : fields) {
        prototype_decode_from_field(header, field);
        header << ";\n";
    }

    header << "\n";

    return true;
}
