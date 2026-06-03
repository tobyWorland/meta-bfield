#include "bfield_builder.hpp"

#include <cassert>
#include <format>
#include <iostream>

void BFieldBuilder::reset() {
    m_field_name.reset();
    m_field_width.reset();
    m_swapped = false;
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
void BFieldBuilder::set_swapped() {
    m_swapped = true;
}
void BFieldBuilder::push_back_part(const BPart &part) {
    m_parts.push_back(part);
}
void BFieldBuilder::push_back_export(const BExport &exp) {
    m_exports.push_back(exp);
}

BField BFieldBuilder::build() {
    if (!m_field_name) {
        throw BFieldBuilderError("Name missing");
    }

    if (!m_field_width) {
        throw BFieldBuilderError("Width missing");
    }

    // TODO: Write test
    if (m_swapped) {
        std::vector<BPart> swapped_parts;
        auto half_way_it = m_parts.begin();
        unsigned half_way_width{};

        swapped_parts.reserve(m_parts.size());

        for (; half_way_it != m_parts.cend(); ++half_way_it) {
            half_way_width += half_way_it->width();

            if (half_way_width > (m_field_width.value() / 2)) {
                throw BFieldBuilderError("Parts can not be split evenly");
            }

            if (half_way_width == (m_field_width.value() / 2)) {
                ++half_way_it;
                break;
            }
        }

        // Cannot use insert here because BPart doesn't have copy assignment
        for (auto it = half_way_it; it != m_parts.end(); ++it) {
            swapped_parts.push_back(std::move(*it));
        }
        for (auto it = m_parts.begin(); it != half_way_it; ++it) {
            swapped_parts.push_back(std::move(*it));
        }

        m_parts.swap(swapped_parts);
    }

    BField result{m_field_name.value(), m_field_width.value(), m_parts, m_exports};

    reset();

    return result;
}
