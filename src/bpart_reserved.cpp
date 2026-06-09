#include "bpart_reserved.hpp"

#include <format>
#include <stdexcept>

BPartReserved::BPartReserved(unsigned width, unsigned reserved)
    : IBPart(width), m_reserved_value{reserved} {
    if (!in_bit_width(reserved, width)) {
        throw std::invalid_argument(
            std::format("Reserved value {} exceeds bit width of {}", reserved, width)
            );
    }
}

BPartReserved::BPartReserved(ReservedValue res_value)
    : BPartReserved(res_value.width, res_value.value) {}

unsigned BPartReserved::reserved_value() const {
    return m_reserved_value;
}
