#pragma once

#include "bexport.hpp"
#include "ibpart.hpp"

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class BField {
    std::string m_name;
    unsigned m_width;
    std::vector<std::unique_ptr<IBPart>> m_parts;
    std::vector<BExport> m_exports;
    std::unordered_set<const IBPart *> m_exported_parts;
    std::unordered_map<const IBPart *, unsigned> m_part_shift_map;

  public:
    BField(std::string name, unsigned width,
           std::vector<std::unique_ptr<IBPart>> parts,
           std::vector<BExport> exports);

    [[nodiscard]] const std::string &name() const;
    [[nodiscard]] unsigned width() const;
    [[nodiscard]] const std::vector<std::unique_ptr<IBPart>> &parts() const;
    [[nodiscard]] unsigned reserved_value() const; // TODO: Test
    [[nodiscard]] bool any_variable_parts() const;
    [[nodiscard]] bool any_reserved_parts() const;
    [[nodiscard]] const std::vector<BExport> exports() const;
    [[nodiscard]] bool is_part_exported(const IBPart *part) const;
    [[nodiscard]] unsigned part_shift(const IBPart *part) const;
};
