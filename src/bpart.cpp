#include "bpart.hpp"

#include <cassert>
#include <format>
#include <stdexcept>

BPart::BPart(std::string name, unsigned width,
             std::string encode_expr, std::string decode_expr)
    : m_name{std::move(name)}, m_width{width},
      m_encode_expr{std::move(encode_expr)},
      m_decode_expr{std::move(decode_expr)} {
    if (m_encode_expr.empty() != m_decode_expr.empty()) {
        throw std::invalid_argument(
            "If part has an encode or decode value, then it must have both");
    }
}

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

bool BPart::has_exprs() const {
    assert(m_encode_expr.empty() == m_decode_expr.empty());
    return !m_encode_expr.empty();
}

std::string BPart::encode_expr(std::string_view name) const {
    assert(has_exprs());

    std::string ret = m_encode_expr;
    auto idx = ret.find('$');

    assert(idx < ret.length());

    ret.replace(idx, 1, name);

    return ret;
}

std::string BPart::decode_expr(std::string_view name) const {
    assert(has_exprs());

    std::string ret = m_decode_expr;
    auto idx = ret.find('$');

    assert(idx < ret.length());

    ret.replace(idx, 1, name);

    return ret;
}
