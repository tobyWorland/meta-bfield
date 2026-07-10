#pragma once

#include <string_view>

class BPartReserved;
class BPartVariable;

class IBPart {
protected:
    unsigned m_width;

    IBPart(unsigned width);
public:
    [[nodiscard]] virtual unsigned width() const;

    [[nodiscard]] const BPartReserved *reserved() const;
    [[nodiscard]] const BPartVariable *variable() const;

    [[nodiscard]] virtual const BPartVariable *variable_name_match(std::string_view name) const;
};
