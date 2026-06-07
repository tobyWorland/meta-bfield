#include "c_generator.hpp"

#include "preludes.hpp"

#include <cassert>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <unordered_set>

// 4 spaces = one indent
constexpr std::string indent(unsigned n=1) {
    return std::string(4*n, ' ');
}

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
    file << struct_name_from_field(field) << " {\n";
    for (const BExport &exp : field.exports()) {
        if (exp.is_passthrough()) {
            const BPart *part = exp.part_refs()[0];
            if (part->is_reserved())
                continue;

            assert(part->width() <= 32);
            file << indent() << type_from_width(part->width()) << " " << part->name() << ";\n";
        } else {
            // assert(!exp.is_signed()); // TODO: Temp disable
            // TODO: handle if signed
            file << indent() << type_from_width(32) << " " << exp.name() << ";\n";
        }
    }
    file << "};\n\n";
}

void prototype_match_from_field(std::fstream &file, const BField &field) {
    file << "bool match_" << field.name() << "(" << type_from_width(field.width()) << " field)";
}

void prototype_encode_from_field(std::fstream &file, const BField &field) {
    file << "unsigned encode_" << field.name();
    file << "(" << type_from_width(field.width()) << " *out";
    if (field.any_variable_parts()) {
        file << ", " << struct_name_from_field(field) << " *parts";
    }
    file << ")";
}

void prototype_decode_from_field(std::fstream &file, const BField &field) {
    file << struct_name_from_field(field);
    file << " decode_" << field.name();
    file << "(" << type_from_width(field.width()) << " field)";
}

void generate_header(std::fstream &header, const std::vector<BField> &fields) {
    for (const BField &field : fields) {
        if (field.any_variable_parts()) {
            struct_from_field(header, field);
        }
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
        if (field.any_variable_parts()) {
            prototype_decode_from_field(header, field);
            header << ";\n";
        }
    }

    header << "\n";
}

void body_match_from_field(std::fstream &source, const BField &field) {
    unsigned width_left = field.width();
    bool first{true};

    prototype_match_from_field(source, field);
    source << " {\n" << indent() << "return ";
    for (const auto &part : field.parts()) {
        if (part->is_reserved()) {
            if (!first) {
                source << " && \\\n" << indent(2);
            }
            first = false;
            unsigned shift = width_left - part->width();
            source << std::format("(BIT_EXTRACT(field, {}, {}) == 0x{:X}U)",
                                  shift, part->width(), part->reserved_value());

            }
        width_left -= part->width();
    }
    source << ";\n}\n";
}

void body_encode_from_field(std::fstream &source, const BField &field) {
    unsigned width_left = field.width();
    std::unordered_set<std::string> exported_parts;

    prototype_encode_from_field(source, field);
    source << " {\n";
    source << indent() << type_from_width(field.width()) << " encoded = 0x"
           << std::hex << field.reserved_value() << std::dec << ";\n";
    for (const BExport &exp : field.exports()) {
        if (!exp.is_passthrough()) {
            unsigned shift = exp.width() + exp.shift();

            // If there is a shift check the shifted part is zerod
            if (exp.shift() > 0) {
                source << indent() << std::format("if (BIT_EXTRACT(parts->{}, 0, {})) {{\n",
                                                  exp.name(), exp.shift());
                source << indent(2) << "return 0;\n" << indent() << "}\n";
            }

            // Check export does not exceed width and if it does then return 0 to signal error
            source << indent() << std::format("if (!UNSIGNED_CHECK_FIT(parts->{} >> {}, {})) {{\n",
                                              exp.name(), exp.shift(), shift);
            source << indent(2) << "return 0;\n" << indent() << "}\n";

            // Create variable for parts that compose the export
            for (const auto &part : exp.part_refs()) {
                exported_parts.insert(part->name());
                shift -= part->width();
                source << indent() << "uint32_t " << part->name() << " = ";
                source << std::format("BIT_EXTRACT(parts->{}, {}, {});\n",
                                      exp.name(), shift, part->width());
            }
        }
    }
    for (const auto &part : field.parts()) {
        if (!part->is_reserved()) {
            unsigned shift = width_left - part->width();
            bool is_export_part = exported_parts.contains(part->name());
            if (!is_export_part) {
                // Check part does not exceed width and if it does then return 0 to signal error
                source << indent() << std::format("if (!UNSIGNED_CHECK_FIT(parts->{}, {})) {{\n",
                                                  part->name(), part->width());
                source << indent(2) << "return 0;\n" << indent() << "}\n";
            }

            // Encode part
            source << indent() << std::format("encoded |= ({}{} << {});\n",
                                              is_export_part ? "" : "parts->", part->name(), shift);
        }

        width_left -= part->width();
    }
    source << indent() << "*out = encoded;\n";
    source << indent() << "return " << field.width() << ";\n";
    source << "}\n";
}

void body_decode_from_field(std::fstream &source, const BField &field) {
    unsigned width_left = field.width();

    prototype_decode_from_field(source, field);
    source << " {\n";
    source << indent() << struct_name_from_field(field) << " result = {};\n";

    for (const auto &part : field.parts()) {
        if (!part->is_reserved()) {
            unsigned shift = width_left - part->width();
            if (field.is_part_exported(part.get())) {
                source << indent()
                       << std::format("result.{} = BIT_EXTRACT(field, {}, {});\n",
                                      part->name(), shift, part->width());
            } else {
                source << indent()
                       << std::format("uint32_t {} = BIT_EXTRACT(field, {}, {});\n",
                                      part->name(), shift, part->width());
            }
        }
        width_left -= part->width();
    }

    for (const BExport &exp : field.exports()) {
        if (!exp.is_passthrough()) {
            unsigned shift = exp.width();
            source << indent() << std::format("uint32_t {} = 0;\n", exp.name());

            for (const auto &part : exp.part_refs()) {
                shift -= part->width();
                source << indent() << std::format("{} |= {} << {};\n", exp.name(), part->name(), shift);
            }


            if (exp.shift() > 0) {
                source << indent() << std::format("result.{} = {} << {};\n", exp.name(), exp.name(), exp.shift());
            } else {
                source << indent() << std::format("result.{} = {};\n", exp.name(), exp.name());
            }
        }
    }

    source << indent() << "return result;\n";
    source << "}\n";
}

void generate_source(std::fstream &source, const std::vector<BField> &fields) {
    for (const BField &field : fields) {
        body_match_from_field(source, field);
    }
    source << "\n";

    for (const BField &field : fields) {
        body_encode_from_field(source, field);
    }
    source << "\n";

    for (const BField &field : fields) {
        if (field.any_variable_parts()) {
            body_decode_from_field(source, field);
        }
    }
    source << "\n";
}

bool generate_fields(const std::string output_basepath, const std::vector<BField> &fields) {
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

    header << "// AUTOGEN - DO NOT EDIT\n\n";
    source << "// AUTOGEN - DO NOT EDIT\n\n";

    header << "#pragma once\n\n" << "#include <stdbool.h>\n#include <stdint.h>\n\n";
    source << "#include \"" << header_file << "\"\n";

    source << c_prelude;

    generate_header(header, fields);
    generate_source(source, fields);

    return true;
}
