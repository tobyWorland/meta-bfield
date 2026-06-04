#include "util.hpp"

#include <format>
#include <stdexcept>

ReservedValue res_from_binstring(const std::string &binstring) {
    ReservedValue result{};

    for (char c : binstring) {
        switch (c) {
        case '1':
        case '0':
            result.width++;
            result.value <<= 1;
            result.value |= c - '0';
            break;
        case ' ':
            break;
        default:
            throw std::invalid_argument(std::format("Unexpected char in binary string {}", c));
        }
    }

    if (result.width > sizeof(result.value)*8) {
        throw std::invalid_argument(std::format("Bit string width {} exceeds maximum {}",
                                                result.width, sizeof(result.value)*8));
    }

    return result;
}
