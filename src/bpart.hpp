#pragma once

#include <string_view>

class BPartReserved;
class BPartVariable;

class BPart {
protected:
    unsigned m_width;

    BPart(unsigned width);
public:
    virtual unsigned width() const;

    const BPartReserved *reserved() const;
    const BPartVariable *variable() const;

    virtual const BPartVariable *variable_name_match(std::string_view name) const;
};
