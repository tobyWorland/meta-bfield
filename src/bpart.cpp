#include "bpart.hpp"

#include <format>
#include <stdexcept>

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
