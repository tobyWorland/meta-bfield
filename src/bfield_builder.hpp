#pragma once

#include "bfield.hpp"

#include <exception>
#include <memory>
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
    std::vector<std::unique_ptr<BPart>> m_parts;
    std::vector<BExpr> m_exprs;
    std::vector<BExport> m_exports;
    bool m_swapped{false};

    // TODO: group these in a struct
    std::optional<std::string> m_new_export_name;
    std::vector<const BPart*> m_new_export_part_refs;
    std::optional<bool> m_new_export_is_signed;
    std::optional<unsigned> m_new_export_optional_shift;

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
    void push_back_expr(std::string name, std::string part_name, std::string encode, std::string decode);

    void export_new();
    void export_set_name(std::string name);
    void export_push_part(std::string part_name);
    void export_set_signed(bool is_signed);
    void export_commit();

    void export_commit_passthrough(std::string part_name);

    BField build();
};
