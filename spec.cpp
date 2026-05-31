#include "spec.hpp"

#include <boost/json.hpp>
#include <fstream>
#include <format>
#include <functional>
#include <iostream>
#include <unordered_map>

// TODO: include the json or field in the error message
// TODO: Remove boost/json.hpp include from the header and hide in the source class

template <typename T>
static T &expect_value_type(boost::system::result<T &> result,
                     const std::string &err) {
    if (!result) {
        throw SpecException(err);
    }
    return result.unsafe_value();
}

static std::string expect_value_string(boost::json::value expected_string, const std::string &err) {
    auto boost_string = expect_value_type(expected_string.try_as_string(), err);
    return {boost_string.begin(), boost_string.end()};
}

static boost::json::value json_from_stream(std::istream &stream) {
    boost::json::stream_parser stream_parser;
    boost::system::error_code err_code;

    stream_parser.reset();

    for (;;) {
        std::string line;
        std::getline(stream, line);
        if (stream.eof()) {
            break;
        }
        stream_parser.write(line, err_code);
        if (err_code) {
            throw SpecException(std::format("JSON Error: {}", err_code.message()));
        }
    }

    return stream_parser.release();
}

void SpecReader::extract_parts(boost::json::value element_parts) {
    auto part_array = expect_value_type(element_parts.try_as_array(),
                                        "Expected parts to be an array");

    for (auto &part_value : part_array) {
        switch (part_value.kind()) {
        case boost::json::kind::object:
        {
            auto object = part_value.get_object();
            if (!object.contains("name")) {
                throw SpecException("Part object is missing a name field");
            }
            if (!object.contains("width")) {
                throw SpecException("Part object is missing a width field");
            }
            auto part_name = expect_value_string(
                object.at("name"), "Expect part name to be a string");

            auto part_width =
                expect_value_type(object.at("width").try_as_int64(),
                                  "Expect part width to be an int64");
            m_field_builder.push_back_part(BPart(std::string(part_name.data()), part_width));
            break;
        }
        case boost::json::kind::string:
        {
            auto reserved_binfield = std::string(part_value.get_string().data());
            m_field_builder.push_back_part(res_from_binstring(reserved_binfield));
            break;
        }
        default:
            throw SpecException(std::format("Expected string or object not: {}", boost::json::serialize(part_value)));
        }
    }

}

void SpecReader::extract_field(boost::json::value element) {
    auto obj = expect_value_type(element.try_as_object(), "Expected top level array to contain field objects");
    for (auto &pair: obj) {
        const static std::unordered_map<boost::json::string, std::function<void(boost::json::value)>> map {
            {"name", [this](boost::json::value value) {
                // std::cout << "Got name: " << value << '\n';
                auto name_string = expect_value_string(value, "Expect field name to be a string");
                m_field_builder.set_field_name(name_string);
            }},
            {"width", [this](boost::json::value value) {
                // std::cout << "Got width: " << value << '\n';
                auto width_i64 = expect_value_type(value.try_as_int64(), "Expect field width to be an int64");
                m_field_builder.set_field_width(width_i64);
            }},
            {"parts", [this](boost::json::value value) {
                 // std::cout << "Got parts: " << value << '\n';
                 extract_parts(value);
             }},
            {"swapped", [this](boost::json::value value) {
                // std::cout << "Got swapped: " << value << '\n';
                auto swapped_bool = expect_value_type(value.try_as_bool(), "Expect field swapped to be a bool");
                if (swapped_bool) {
                    m_field_builder.set_swapped();
                }
            }},
        };

        auto it = map.find(pair.key());
        if (it != map.end()) {
            it->second(pair.value());
        } else {
            throw SpecException(std::format("Unknown key in field object '{}'", pair.key().data()));
        }
    }
}

std::vector<BField> SpecReader::fields_from_json(boost::json::value &value) {
    std::vector<BField> fields;

    auto field_arr = expect_value_type(value.try_as_array(), "Expected top level json to be an array");
    for (auto &element : field_arr) {
        m_field_builder.reset();
        extract_field(element);
        fields.emplace_back(m_field_builder.build());
    }

    return fields;
}

std::vector<BField> SpecReader::read_from_spec(std::filesystem::path spec_path) {
    std::fstream file{spec_path, std::ios_base::in};
    if (!file.is_open()) {
        throw SpecException(std::format("Failed to open '{}'", spec_path.string()));
    }

    auto json = json_from_stream(file);
    file.close();

    return fields_from_json(json);
}
