#include "bfield.hpp"

#include <cassert>
#include <format>
#include <stdexcept>

BPart::BPart(const std::string &name, unsigned width)
    : m_name{name}, m_width{width} {

}

BPart::BPart(unsigned width, unsigned reserved)
    : m_name{std::nullopt}, m_width{width}, m_reserved_value{reserved} {
    if (!in_bit_width(reserved, width)) {
        throw std::invalid_argument(
            std::format("Reserved value {} exceeds bit width of {}", reserved, width)
            );
    }
}

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

bool BPart::reserved() const {
    return m_name == std::nullopt;
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
