#pragma once

#include <string>
#include <vector>
#include <optional>

template<typename T> T bit(T n) {
    return (static_cast<T>(1) << n);
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

class BField {
    const std::string m_name;
    const unsigned m_width;
    std::vector<BPart> m_parts;
public:
    BField(const std::string &name, unsigned width, const std::vector<BPart> &parts);

    const std::string &name() const;
    unsigned width() const;
    const std::vector<BPart> parts() const;
    unsigned reserved_value() const; // TODO: Test
};
