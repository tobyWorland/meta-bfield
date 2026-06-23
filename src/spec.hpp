#pragma once

#include "bfield.hpp"

#include <exception>
#include <filesystem>
#include <vector>

class SpecException: public std::exception {
    std::string m_message;
public:
    SpecException(const std::string &message)
        : m_message{message} { }
    virtual const char *what() const noexcept override { return m_message.c_str(); }
};

class SpecReader {
    struct pimpl;

public:
    std::vector<BField> read_from_spec(std::filesystem::path spec_path);
};
