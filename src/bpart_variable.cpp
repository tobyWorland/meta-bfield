#include "bpart_variable.hpp"

#include <cassert>
#include <stdexcept>

BPartVariable::BPartVariable(std::string name, unsigned width,
                             std::string encode_expr, std::string decode_expr)
    : IBPart(width), m_name{std::move(name)}, m_encode_expr{std::move(encode_expr)},
      m_decode_expr{std::move(decode_expr)} {
    if (m_encode_expr.empty() != m_decode_expr.empty()) {
        throw std::invalid_argument(
            "If part has an encode or decode value, then it must have both");
    }
}

const BPartVariable *BPartVariable::variable_name_match(std::string_view name) const {
    if (name == m_name) {
        return this;
    } else {
        return nullptr;
    }
}

const std::string &BPartVariable::name() const {
    return m_name;
}

bool BPartVariable::has_exprs() const {
    assert(m_encode_expr.empty() == m_decode_expr.empty());
    return !m_encode_expr.empty();
}

std::string BPartVariable::encode_expr(std::string_view name) const {
    assert(has_exprs());

    std::string ret = m_encode_expr;
    auto idx = ret.find('$');

    assert(idx < ret.length());

    ret.replace(idx, 1, name);

    return ret;
}

std::string BPartVariable::decode_expr(std::string_view name) const {
    assert(has_exprs());

    std::string ret = m_decode_expr;
    auto idx = ret.find('$');

    assert(idx < ret.length());

    ret.replace(idx, 1, name);

    return ret;
}
