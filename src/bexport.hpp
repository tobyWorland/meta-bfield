#pragma once

#include <string>
#include <vector>

class BExport {
    std::string m_name;
    std::vector<std::string> m_part_names;
    bool m_signed;

public:
    BExport(const std::string &passthrough_part_name);
    BExport(const std::string &name, const std::vector<std::string> &part_names, bool is_signed);

    const std::string &name() const;
    const std::vector<std::string> part_names() const;
    bool is_passthrough() const;
    bool is_signed() const;
};
