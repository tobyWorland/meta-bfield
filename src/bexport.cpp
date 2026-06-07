#include "bexport.hpp"

#include <cassert>

BExport::BExport(const BPart *passthrough_part)
    : m_name{passthrough_part->name()}, m_width{passthrough_part->width()}, m_part_refs{passthrough_part},
      m_signed{}, m_passthrough{true} {}
BExport::BExport(const std::string &name, const std::vector<const BPart *> &part_refs, bool is_signed, unsigned shift)
    : m_name{name}, m_width{0}, m_part_refs{part_refs}, m_signed{is_signed},
      m_passthrough{false}, m_shift{shift} {
    assert(!m_name.empty());
    assert(!m_part_refs.empty());
    for (const auto *part : m_part_refs) {
        m_width += part->width();
    }
}

const std::string &BExport::name() const {
    return m_name;
}
unsigned BExport::width() const {
    return m_width;
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
unsigned BExport::shift() const {
    return m_shift;
}
