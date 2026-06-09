#include "bfield_builder.hpp"

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

void BFieldBuilder::export_new() {
    m_new_export_name.reset();
    m_new_export_part_refs.clear();
    m_new_export_is_signed.reset();
    m_new_export_optional_shift.reset();
}
void BFieldBuilder::export_set_name(std::string name) {
    if (m_new_export_name) {
        throw BFieldBuilderError(std::format("{}({}) called when name is already set to {}",
                                             __func__, name, *m_new_export_name));
    }
    m_new_export_name = std::move(name);
}
void BFieldBuilder::export_push_part(std::string part_name) {
    // If the optional shift is set then we cannot accept any more parts
    // The shift must occur at the end of the export's part list
    if (m_new_export_optional_shift) {
        throw BFieldBuilderError("Export cannot accept any more parts after a part of all 0s has been used");
    }

    // Handle the all zero part
    if (std::all_of(part_name.cbegin(), part_name.cend(), [](char c) { return c == '0'; })) {
        auto zero_count = part_name.size();
        m_new_export_optional_shift = zero_count;
        return;
    }

    // Look for a variable part with the name we want
    for (const auto &part : m_parts) {
        if (const BPartVariable *var_part = part.get()->variable_name_match(part_name)) {
            m_new_export_part_refs.push_back(var_part);
            return;
        }
    }

    // Couldn't find a part in m_parts called part_name
    throw BFieldBuilderError(std::format("Cannot export part '{}' as it doesn't exist.", part_name));
}
void BFieldBuilder::export_set_signed(bool is_signed) {
    if (m_new_export_is_signed) {
        throw BFieldBuilderError(std::format("{}({}) called when is_signed is already set to {}",
                                             __func__, is_signed, *m_new_export_is_signed));
    }
    m_new_export_is_signed = is_signed;
}
void BFieldBuilder::export_commit() {
    if (!m_new_export_name) {
        throw BFieldBuilderError("Attempt to export without an export name");
    }
    if (m_new_export_part_refs.empty()) {
        throw BFieldBuilderError("Attempt to export without any parts");
    }
    if (!m_new_export_is_signed) {
        throw BFieldBuilderError("Attempt to export without is signed set");
    }

    unsigned shift = m_new_export_optional_shift.value_or(0);

    auto e = BExport(*m_new_export_name, m_new_export_part_refs, *m_new_export_is_signed, shift);
    m_exports.push_back(std::move(e));
}

void BFieldBuilder::export_commit_passthrough(std::string part_name) {
    const BPartVariable *passthrough_part = nullptr;

    for (const auto &part : m_parts) {
        if (const BPartVariable *varpart = part.get()->variable_name_match(part_name)) {
            passthrough_part = varpart;
            break;
        }
    }

    if (passthrough_part == nullptr) {
        throw BFieldBuilderError(std::format("Cannot export pass-through part '{}' as it doesn't exist.", part_name));
    }

    m_exports.push_back(BExport(passthrough_part));
}

BField BFieldBuilder::build() {
    if (!m_field_name) {
        throw BFieldBuilderError("Name missing");
    }

    if (!m_field_width) {
        throw BFieldBuilderError("Width missing");
    }

    export_new(); // Clear export in progress so parts don't contain stale pointers

    // TODO: Write test
    if (m_swapped) {
        std::vector<std::unique_ptr<IBPart>> swapped_parts;
        auto half_way_it = m_parts.begin();
        unsigned half_way_width{};

        swapped_parts.reserve(m_parts.size());

        for (; half_way_it != m_parts.cend(); ++half_way_it) {
            const IBPart *part = half_way_it->get();
            half_way_width += part->width();

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

    BField result{m_field_name.value(), m_field_width.value(), std::move(m_parts), std::move(m_exports)};

    reset();

    return result;
}
