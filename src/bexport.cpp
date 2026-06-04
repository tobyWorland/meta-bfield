#include "bexport.hpp"

#include <cassert>

BExport::BExport(const std::string &passthrough_part_name)
    : m_name{passthrough_part_name}, m_part_refs{}, m_signed{} {
    assert(!m_name.empty());
}
BExport::BExport(const std::string &name, const std::vector<const BPart*> &part_refs, bool is_signed)
    : m_name{name}, m_part_refs{part_refs}, m_signed{is_signed} {
    assert(!m_name.empty());
    assert(!m_part_refs.empty());
}

const std::string &BExport::name() const {
    return m_name;
}
const std::vector<const BPart*> BExport::part_refs() const {
    assert(!is_passthrough());
    return m_part_refs;
}
bool BExport::is_passthrough() const {
    return m_part_refs.empty();
}
bool BExport::is_signed() const {
    return m_signed;
}
