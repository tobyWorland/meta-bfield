#include "bexport.hpp"

#include <cassert>

BExport::BExport(const std::string &passthrough_part_name)
    : m_name{passthrough_part_name}, m_part_names{}, m_signed{} {
    assert(!m_name.empty());
}
BExport::BExport(const std::string &name, const std::vector<std::string> &part_names, bool export_signed)
    : m_name{name}, m_part_names{part_names}, m_signed{export_signed} {
    assert(!m_name.empty());
    assert(!m_part_names.empty());
}

const std::string &BExport::name() const {
    return m_name;
}
const std::vector<std::string> BExport::part_names() const {
    assert(!is_passthrough());
    return m_part_names;
}
bool BExport::is_passthrough() const {
    return m_part_names.empty();
}
bool BExport::is_signed() const {
    return m_signed;
}
