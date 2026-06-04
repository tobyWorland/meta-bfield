#pragma once

#include "util.hpp"

class BPart {
    const std::string m_name;
    const unsigned m_width;
    const unsigned m_reserved_value = 0;
public:
    BPart(const std::string &name, unsigned width);
    BPart(unsigned width, unsigned reserved);
    BPart(ReservedValue res_value);

    const std::string &name() const;
    unsigned width() const;
    bool is_reserved() const;
    unsigned reserved_value() const;
};
