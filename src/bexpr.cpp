#include "bexpr.hpp"

#include <cassert>
#include <format>
#include <stdexcept>

#include <iostream>

BExpr::BExpr(std::string name, std::string part_name, std::string encode_expr,
             std::string decode_expr)
    : m_name(std::move(name)), m_part_name(std::move(part_name)),
      m_encode_expr(std::move(encode_expr)), m_decode_expr(std::move(decode_expr)) {
    assert(!m_name.empty());
    assert(!m_part_name.empty());
    assert(!m_encode_expr.empty());
    assert(!m_decode_expr.empty());

    // TODO: Should sanitise expressions and make them cross language (lex and parse them?)
    replace_in_expr(m_encode_expr, '$', m_part_name);
    replace_in_expr(m_decode_expr, '$', m_name);
}

void BExpr::replace_in_expr(std::string &expr, char c, std::string_view with) {
    auto idx = expr.find(c);
    if (idx >= expr.length()) {
        throw std::invalid_argument(std::format("Expr expected character {} in expr {}", c, expr));
    }

    expr.replace(idx, 1, with);
}

const std::string &BExpr::name() const {
    return m_name;
}
const std::string &BExpr::part_name() const {
    return m_part_name;
}
const std::string &BExpr::encode_expr() const {
    return m_encode_expr;
}
const std::string &BExpr::decode_expr() const {
    return m_decode_expr;
}
