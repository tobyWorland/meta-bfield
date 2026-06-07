#pragma once

#include <string>

class BExpr {
    std::string m_name;
    std::string m_part_name;
    std::string m_encode_expr;
    std::string m_decode_expr;

    void replace_in_expr(std::string &expr, char c, std::string_view with);

  public:
    BExpr(std::string name, std::string part_name, std::string encode_expr,
          std::string decode_expr);

    const std::string &name() const;
    const std::string &part_name() const;
    const std::string &encode_expr() const;
    const std::string &decode_expr() const;
};
