#pragma once

#include "bfield.hpp"

#include <exception>

class BFieldBuilderError : public std::exception {
    std::string m_message;

public:
    BFieldBuilderError(const std::string &message) : m_message{message} { }
    virtual const char *what() const noexcept { return m_message.c_str(); }
};

class BFieldBuilder {
    std::optional<std::string> m_field_name;
    std::optional<unsigned> m_field_width;
    std::vector<BPart> m_parts;

public:
    void reset();

    void set_field_name(const std::string &name);
    void set_field_width(unsigned width);
    void push_back_part(const BPart &part);

    BField build();
};
