#include "bfield_builder.hpp"

#include <cassert>
#include <format>
#include <iostream>

// TODO: Make asserts into throws

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
    m_parts.push_back(std::make_unique<BPart>(part));
}

void BFieldBuilder::export_new() {
    m_new_export_name.reset();
    m_new_export_part_refs.clear();
    m_new_export_is_signed.reset();
}
void BFieldBuilder::export_set_name(std::string name) {
    assert(!m_new_export_name);
    m_new_export_name = std::move(name);
}
void BFieldBuilder::export_push_part(std::string part_name) {
    auto it = std::find_if(m_parts.cbegin(), m_parts.cend(),
                           [&part_name](const auto &part) { return part->name() == part_name; });

    if (it == m_parts.cend()) {
        throw BFieldBuilderError(std::format("Cannot export part '{}' as it doesn't exist.", part_name));
    }

    m_new_export_part_refs.push_back(it->get());
}
void BFieldBuilder::export_set_signed(bool is_signed) {
    assert(!m_new_export_is_signed);
    m_new_export_is_signed = is_signed;
}
void BFieldBuilder::export_commit() {
    assert(m_new_export_name);
    assert(!m_new_export_part_refs.empty());
    assert(m_new_export_is_signed);

    auto e = BExport(*m_new_export_name, m_new_export_part_refs, *m_new_export_is_signed);
    m_exports.push_back(std::move(e));
}

void BFieldBuilder::export_commit_passthrough(std::string part_name) {
    auto it = std::find_if(m_parts.cbegin(), m_parts.cend(),
                           [&part_name](const auto &part) { return part->name() == part_name; });

    if (it == m_parts.cend()) {
        throw BFieldBuilderError(std::format("Cannot export pass-through part '{}' as it doesn't exist.", part_name));
    }

    m_exports.push_back(BExport(it->get()));
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
        std::vector<std::unique_ptr<BPart>> swapped_parts;
        auto half_way_it = m_parts.begin();
        unsigned half_way_width{};

        swapped_parts.reserve(m_parts.size());

        for (; half_way_it != m_parts.cend(); ++half_way_it) {
            const BPart *part = half_way_it->get();
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
