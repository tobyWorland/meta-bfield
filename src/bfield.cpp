#include "bfield.hpp"

#include <cassert>
#include <format>
#include <stdexcept>

BField::BField(const std::string &name, unsigned width,
               std::vector<std::unique_ptr<BPart>> &&parts,
               std::vector<BExport> &&exports)
    : m_name{name}, m_width{width}, m_parts{std::move(parts)}, m_exports{std::move(exports)} {

    if (m_exports.empty()) {
        // No exports - export all parts
        for (const auto &part : m_parts) {
            m_exports.push_back(BExport(part.get()));
            m_exported_parts.insert(part.get());
        }
    } else {
        for (const auto &exp : m_exports) {
            if (exp.is_passthrough()) {
                m_exported_parts.insert(exp.part_refs()[0]);
            }
        }
    }

    unsigned part_width_sum{0};
    for (const auto &part : m_parts) {
        part_width_sum += part->width();
    }
    if (part_width_sum != m_width) {
        throw std::invalid_argument(
            std::format(
                "Part widths only sum to {} instead of the field width {}",
                part_width_sum, m_width));
    }
}

const std::string &BField::name() const {
    return m_name;
}
unsigned BField::width() const {
    return m_width;
}
const std::vector<std::unique_ptr<BPart>> &BField::parts() const {
    return m_parts;
}
unsigned BField::reserved_value() const {
    unsigned value{0};
    for (const auto &part : m_parts) {
        value <<= part->width();
        if (part->is_reserved()) {
            value |= part->reserved_value();
        }
    }
    return value;
}

bool BField::any_variable_parts() const {
    // TODO: Could be !m_exports.empty()?
    return std::any_of(m_parts.cbegin(), m_parts.cend(), [](auto &part) {
        return !part->is_reserved();
    });
}

bool BField::any_reserved_parts() const {
    return std::any_of(m_parts.cbegin(), m_parts.cend(), [](auto &part) {
        return part->is_reserved();
    });
}

const std::vector<BExport> BField::exports() const {
    return m_exports;
}

bool BField::is_part_exported(const BPart *part) const {
    return m_exported_parts.contains(part);
}
