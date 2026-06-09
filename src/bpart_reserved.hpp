#pragma once

#include "bpart.hpp"
#include "util.hpp"

class BPartReserved : public BPart {
    unsigned m_reserved_value = 0;

public:
    BPartReserved(unsigned width, unsigned reserved);
    BPartReserved(ReservedValue res_value);

    unsigned reserved_value() const;
};
