#include "bfield.hpp"

#include <cassert>
#include <format>
#include <stdexcept>

ReservedValue res_from_binstring(const std::string &binstring) {
    ReservedValue result{};

    // TODO: Check width does not exceed bit width of value
    for (char c : binstring) {
        switch (c) {
        case '1':
        case '0':
            result.width++;
            result.value <<= 1;
            result.value |= c - '0';
            break;
        case ' ':
            break;
        default:
            throw std::invalid_argument(std::format("Unexpected char in binary string {}", c));
        }
    }

    return result;
}

BPart::BPart(const std::string &name, unsigned width)
    : m_name{name}, m_width{width} {}

BPart::BPart(unsigned width, unsigned reserved)
    : m_name{}, m_width{width}, m_reserved_value{reserved} {
    if (!in_bit_width(reserved, width)) {
        throw std::invalid_argument(
            std::format("Reserved value {} exceeds bit width of {}", reserved, width)
            );
    }
}

BPart::BPart(ReservedValue res_value)
    : BPart(res_value.width, res_value.value) {}

const std::string &BPart::name() const {
    const static std::string reserved{"Reserved"};

    if (m_name.empty()) {
        return reserved;
    } else {
        return m_name;
    }
}

unsigned BPart::width() const {
    return m_width;
}

bool BPart::is_reserved() const {
    return m_name.empty();
}

unsigned BPart::reserved_value() const {
    return m_reserved_value;
}

BExport::BExport(const std::string &passthrough_part_name)
    : m_name{passthrough_part_name}, m_part_names{}, m_signed{} {
    assert(!m_name.empty());
}
BExport::BExport(const std::string &name, const std::vector<std::string> &part_names, bool export_signed)
    : m_name{name}, m_part_names{part_names}, m_signed{export_signed} {
    assert(!m_name.empty());
    assert(!m_part_names.empty());
}

const std::string &BExport::name() const {
    return m_name;
}
const std::vector<std::string> BExport::part_names() const {
    assert(!is_passthrough());
    return m_part_names;
}
bool BExport::is_passthrough() const {
    return m_part_names.empty();
}
bool BExport::is_signed() const {
    return m_signed;
}

BField::BField(const std::string &name, unsigned width,
               const std::vector<BPart> &parts,
               const std::vector<BExport> &exports)
    : m_name{name}, m_width{width}, m_parts{parts}, m_exports{exports} {

    if (m_exports.empty()) {
        // No exports - export all parts
        for (const auto &part : m_parts) {
            m_exports.push_back(BExport(part.name()));
        }
    }

    unsigned part_width_sum{0};
    for (const BPart &part : m_parts) {
        part_width_sum += part.width();
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
const std::vector<BPart> BField::parts() const {
    return m_parts;
}
unsigned BField::reserved_value() const {
    unsigned value{0};
    for (const BPart &part : m_parts) {
        value <<= part.width();
        if (part.is_reserved()) {
            value |= part.reserved_value();
        }
    }
    return value;
}

bool BField::any_variable_parts() const {
    // TODO: Could be !m_exports.empty()?
    return std::any_of(m_parts.cbegin(), m_parts.cend(),
                       [](auto &part) { return !part.is_reserved(); }
        );
}

const std::vector<BExport> BField::exports() const {
    return m_exports;
}

// TODO: Reference to BField should be included in BExport instead
const BPart &BField::get_passthrough_part(const BExport &exp) const {
    for (const BPart &part: m_parts) {
        if (part.name() == exp.name()) {
            return part;
        }
    }
    assert(false);
    std::terminate();
}

// TODO: should be a better way
const BPart &BField::get_part_by_name(const std::string &part_name) const {
    auto found_part_it =
        std::find_if(m_parts.cbegin(), m_parts.cend(),
                     [&part_name](const BPart &part) {
                         return part.name() == part_name;
                     });
    if (found_part_it == m_parts.cend()) {
        assert(false);
        std::terminate();
    }
    return *found_part_it;
}

// TODO: Should be a better way
unsigned BField::get_export_width(const BExport &exp) const {
    unsigned width{0};
    for (const auto &name : exp.part_names()) {
        width += get_part_by_name(name).width();
    }
    return width;
}

bool BField::is_part_exported(const std::string &part_name) const {
    for (const auto &exp : m_exports) {
        if (exp.name() == part_name) {
            return true;
        }
    }
    return false;
}
