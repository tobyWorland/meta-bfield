#pragma once

#include "bexport.hpp"
#include "bpart.hpp"

#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

class BField {
    std::string m_name;
    unsigned m_width;
    std::vector<std::unique_ptr<BPart>> m_parts;
    std::vector<BExport> m_exports;
    std::unordered_set<const BPart *> m_exported_parts;

  public:
    BField(const std::string &name, unsigned width,
           std::vector<std::unique_ptr<BPart>> &&parts,
           std::vector<BExport> &&exports);

    const std::string &name() const;
    unsigned width() const;
    const std::vector<std::unique_ptr<BPart>> &parts() const;
    unsigned reserved_value() const; // TODO: Test
    bool any_variable_parts() const;
    const std::vector<BExport> exports() const;
    bool is_part_exported(const BPart *part) const;
};
