#include "bfield_builder.hpp"

#include <format>

void BFieldBuilder::reset() {
    m_field_name.reset();
    m_field_width.reset();
    m_parts.clear();
}

void BFieldBuilder::set_field_name(const std::string &name) {
    if (auto fname = m_field_name) {
        throw BFieldBuilderError(std::format("Field name already set to {}", *fname));
    }
    m_field_name = name;
}
void BFieldBuilder::set_field_width(unsigned width) {
    if (auto fwidth = m_field_width) {
        throw BFieldBuilderError(std::format("Field width already set to {}", *fwidth));
    }
    m_field_width = width;
}
void BFieldBuilder::push_back_part(const BPart &part) {
    m_parts.push_back(part);
}

BField BFieldBuilder::build() {
    if (!m_field_name) {
        throw BFieldBuilderError("Name missing");
    }

    if (!m_field_width) {
        throw BFieldBuilderError("Width missing");
    }

    BField result{m_field_name.value(), m_field_width.value(), m_parts};

    reset();

    return result;
}
