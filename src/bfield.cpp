#include "bfield.hpp"

#include "bpart_reserved.hpp"

#include <cassert>
#include <format>
#include <stdexcept>

BField::BField(std::string name, unsigned width,
               std::vector<std::unique_ptr<BPart>> parts,
               std::vector<BExport> exports)
    : m_name{std::move(name)}, m_width{width}, m_parts{std::move(parts)},
      m_exports{std::move(exports)} {
    if (m_exports.empty()) {
        // No exports - export all parts
        for (const auto &part : m_parts) {
            if (const auto *ptr = part.get()->variable()) {
                m_exports.push_back(BExport(ptr));
                m_exported_parts.insert(ptr);
            }
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

    // TODO: Cache shifts
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
unsigned BField::reserved_value() const { // TODO: Could be cached in constructor
    unsigned value{0};
    for (const auto &part : m_parts) {
        value <<= part->width();
        if (const BPartReserved *reserved = part->reserved()) {
            value |= reserved->reserved_value();
        }
    }
    return value;
}

bool BField::any_variable_parts() const {
    // TODO: Could be !m_exports.empty()?
    return std::any_of(m_parts.cbegin(), m_parts.cend(), [](auto &part) {
        return part->variable() != nullptr;
    });
}

bool BField::any_reserved_parts() const {
    return std::any_of(m_parts.cbegin(), m_parts.cend(), [](auto &part) {
        return part->reserved() != nullptr;
    });
}

const std::vector<BExport> BField::exports() const {
    return m_exports;
}

bool BField::is_part_exported(const BPart *part) const {
    return m_exported_parts.contains(part);
}
