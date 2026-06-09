#include "ibpart.hpp"

#include "bpart_reserved.hpp"
#include "bpart_variable.hpp"

IBPart::IBPart(unsigned width) : m_width{width} { }

unsigned IBPart::width() const {
    return m_width;
}

const BPartReserved *IBPart::reserved() const {
    return dynamic_cast<const BPartReserved *>(this);
}

const BPartVariable *IBPart::variable() const {
    return dynamic_cast<const BPartVariable *>(this);
}

const BPartVariable *IBPart::variable_name_match(std::string_view name) const {
    return nullptr;
}
