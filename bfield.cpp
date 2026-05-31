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
    : m_name{std::nullopt}, m_width{width}, m_reserved_value{reserved} {
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

    if (auto &name = m_name) {
        return *name;
    }
    return reserved;
}

unsigned BPart::width() const {
    return m_width;
}

bool BPart::is_reserved() const {
    return m_name == std::nullopt;
}

unsigned BPart::reserved_value() const {
    return m_reserved_value;
}

BField::BField(const std::string &name, unsigned width,
               const std::vector<BPart> &parts)
    : m_name{name}, m_width{width}, m_parts{parts} {

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
    return std::any_of(m_parts.cbegin(), m_parts.cend(),
                       [](auto &part) { return !part.is_reserved(); }
        );
}
