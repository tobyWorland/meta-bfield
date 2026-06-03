#pragma once

#include "bexport.hpp"
#include "bpart.hpp"

#include <string>
#include <vector>

class BField {
    const std::string m_name;
    const unsigned m_width;
    std::vector<BPart> m_parts;
    std::vector<BExport> m_exports;

  public:
    BField(const std::string &name, unsigned width,
           const std::vector<BPart> &parts,
           const std::vector<BExport> &exports = {});

    const std::string &name() const;
    unsigned width() const;
    const std::vector<BPart> parts() const;
    unsigned reserved_value() const; // TODO: Test
    bool any_variable_parts() const;
    const std::vector<BExport> exports() const;
    const BPart &get_passthrough_part(const BExport &exp) const;
    const BPart &get_part_by_name(const std::string &part_name) const;
    unsigned get_export_width(const BExport &exp) const;
    bool is_part_exported(const std::string &part_name) const;
};
