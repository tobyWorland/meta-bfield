#pragma once

#include "util.hpp"

class BPart {
    std::string m_name;
    unsigned m_width;
    unsigned m_reserved_value = 0;
    std::string m_encode_expr;
    std::string m_decode_expr;
public:
    BPart(std::string name, unsigned width, std::string encode_expr, std::string decode_expr);
    BPart(unsigned width, unsigned reserved);
    BPart(ReservedValue res_value);

    const std::string &name() const;
    unsigned width() const;
    bool is_reserved() const;
    unsigned reserved_value() const;
    bool has_exprs() const;
    std::string encode_expr(std::string_view name) const;
    std::string decode_expr(std::string_view name) const;
};
