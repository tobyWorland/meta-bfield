#include "bexport.hpp"

#include <cassert>

BExport::BExport(const BPart *passthrough_part)
    : m_name{passthrough_part->name()}, m_part_refs{passthrough_part},
      m_signed{}, m_passthrough{true} {}
BExport::BExport(const std::string &name, const std::vector<const BPart *> &part_refs, bool is_signed)
    : m_name{name}, m_part_refs{part_refs}, m_signed{is_signed},
      m_passthrough{false} {
    assert(!m_name.empty());
    assert(!m_part_refs.empty());
}

const std::string &BExport::name() const {
    return m_name;
}
unsigned BExport::width() const {
    unsigned width{0};
    for (const auto *part : part_refs()) {
        width += part->width();
    }
    return width;
}
const std::vector<const BPart*> BExport::part_refs() const {
    return m_part_refs;
}
bool BExport::is_passthrough() const {
    return m_passthrough;
}
bool BExport::is_signed() const {
    return m_signed;
}
