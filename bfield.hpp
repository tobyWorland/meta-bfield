#pragma once

#include <string>
#include <vector>
#include <optional>

template<typename T> T bit(T n) {
    return (1ULL << n);
}

template <typename T> bool in_bit_width(T value, T bit_width) {
    return (value & ~(bit(bit_width) - 1)) == 0;
}

struct ReservedValue {
    unsigned width;
    unsigned value;
};

// TODO: Make static of BPart?
ReservedValue res_from_binstring(const std::string &binstring);

class BPart {
    const std::optional<std::string> m_name;
    const unsigned m_width;
    const unsigned m_reserved_value = 0;
public:
    BPart(const std::string &name, unsigned width);
    BPart(unsigned width, unsigned reserved);
    BPart(ReservedValue res_value);

    const std::string &name() const;
    unsigned width() const;
    bool is_reserved() const;
    unsigned reserved_value() const;
};

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
};
