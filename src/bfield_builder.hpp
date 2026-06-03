#pragma once

#include "bfield.hpp"

#include <exception>
#include <optional>

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
    std::vector<BExport> m_exports;
    bool m_swapped{false};

public:
    void reset();

    void set_field_name(const std::string &name);
    void set_field_width(unsigned width);
    // Swaps half of the parts with the other.
    // Used in the case of thumb instructions where 32bit instructions are
    // actually just 2 16bit half words, which isn't the same as a single word
    // on little endian. To get the correct encoding we can just swap the half
    // words around, rather than doing this in the generated code just cut the
    // parts in half and swap the halves instead.
    void set_swapped(); // TODO: Test
    void push_back_part(const BPart &part);
    void push_back_export(const BExport &exp);

    BField build();
};
