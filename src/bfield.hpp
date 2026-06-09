#pragma once

#include "bexport.hpp"
#include "ibpart.hpp"

#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

class BField {
    std::string m_name;
    unsigned m_width;
    std::vector<std::unique_ptr<IBPart>> m_parts;
    std::vector<BExport> m_exports;
    std::unordered_set<const IBPart *> m_exported_parts;

  public:
    BField(std::string name, unsigned width,
           std::vector<std::unique_ptr<IBPart>> parts,
           std::vector<BExport> exports);

    const std::string &name() const;
    unsigned width() const;
    const std::vector<std::unique_ptr<IBPart>> &parts() const;
    unsigned reserved_value() const; // TODO: Test
    bool any_variable_parts() const;
    bool any_reserved_parts() const;
    const std::vector<BExport> exports() const;
    bool is_part_exported(const IBPart *part) const;
};
