#pragma once

#include "ibpart.hpp"

#include <string>

class BPartVariable : public IBPart {
    std::string m_name;
    std::string m_encode_expr;
    std::string m_decode_expr;

public:
    BPartVariable(std::string name, unsigned width, std::string encode_expr, std::string decode_expr);

    virtual const BPartVariable *variable_name_match(std::string_view name) const override;

    const std::string &name() const;
    bool has_exprs() const;
    std::string encode_expr(std::string_view name) const;
    std::string decode_expr(std::string_view name) const;
};
