#include "bpart.hpp"

#include "bpart_reserved.hpp"
#include "bpart_variable.hpp"

BPart::BPart(unsigned width) : m_width{width} { }

unsigned BPart::width() const {
    return m_width;
}

const BPartReserved *BPart::reserved() const {
    return dynamic_cast<const BPartReserved *>(this);
}

const BPartVariable *BPart::variable() const {
    return dynamic_cast<const BPartVariable *>(this);
}

const BPartVariable *BPart::variable_name_match(std::string_view name) const {
    return nullptr;
}
