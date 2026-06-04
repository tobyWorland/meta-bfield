#pragma once

#include <string>

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
