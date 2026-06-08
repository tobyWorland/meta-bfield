#pragma once

#include "bfield.hpp"
#include "bfield_builder.hpp"

#include <boost/json.hpp>
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
    BFieldBuilder m_field_builder;

    void extract_parts(boost::json::value element_parts);
    void extract_exports(boost::json::value element_exports);
    void extract_field(boost::json::value element);
    std::vector<BField> fields_from_json(boost::json::value &value);
  public:
    std::vector<BField> read_from_spec(std::filesystem::path spec_path);
};
