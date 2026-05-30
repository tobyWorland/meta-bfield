#pragma once

#include <string>
#include <vector>
#include <optional>

class BPart {
    const std::optional<std::string> m_name;
    const unsigned m_width;
    const unsigned m_reserved_value = 0;
public:
    BPart(const std::string &name, unsigned width);
    BPart(unsigned width, unsigned reserved);

    const std::string &name() const;
    unsigned width() const;
    bool reserved() const;
};

class BField {
    const std::string m_name;
    const unsigned m_width;
    std::vector<BPart> m_parts;
public:
    BField(const std::string &name, unsigned width, const std::vector<BPart> &parts);

    const std::string &name() const;
    unsigned width() const;
    const std::vector<BPart> parts() const;
};
