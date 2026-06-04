#pragma once

#include "bpart.hpp"

#include <string>
#include <vector>

class BExport {
    std::string m_name;
    std::vector<const BPart*> m_part_refs;
    bool m_signed;
    bool m_passthrough;

public:
    BExport(const BPart *passthrough_part);
    BExport(const std::string &name, const std::vector<const BPart*> &part_refs, bool is_signed);

    const std::string &name() const;
    unsigned width() const;
    const std::vector<const BPart*> part_refs() const;
    bool is_passthrough() const;
    bool is_signed() const;
};
