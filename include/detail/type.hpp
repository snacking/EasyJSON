/***
* ==++==
* By Lisw
* 
* ==--==
* =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
* Parser
*
* Main public header file for json value type
* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
***/

#pragma once

// std::string
#include <string>
// std::shared_ptr
#include <memory>
// std::map
#include <map>
// std::unordered_map
#include <unordered_map>
// std::variant
#include <variant>
// serialze
#include "./output/serializer.hpp"
// base64
#include "./conversions/base64.hpp"

namespace sw {

class value;
namespace detail {

enum class type : std::uint8_t
{
    null,             ///< null value
    boolean,          ///< boolean value
    string,           ///< string value
    number_integer,   ///< number value (signed integer)
    number_unsigned,  ///< number value (unsigned integer)
    number_float,     ///< number value (floating-point)
    binary,           ///< binary array (ordered collection of bytes)
    array,            ///< array (ordered collection of values)
    object,           ///< object (set of unordered key/value pairs)
    ordered_object,   ///< object (set of ordered key/value pairs)
    discarded         ///< discarded by the parser callback function
};

using boolean_t = bool;
using string_t = std::string;
using number_t = union {
    std::int64_t number_integer;
    std::uint64_t number_unsigned;
    double number_float;
};
using binary_t = std::vector<uint8_t>;
using array_t = std::vector<value>;
using object_t = std::variant<
    std::map<string_t, value>,
    std::unordered_map<string_t, value>
>;

/*

JSON type | valueT type    | used type
--------- | ---------------| ------------------------
null      | null           | *no value is stored*
boolean   | boolean_t      | @ref boolean_t
string    | string_t       | pointer to @ref string_t
number    | number_t       | @ref number_t
binary    | binary_t       | pointer to @ref binary_t
array     | array_t        | pointer to @ref array_t
object    | object_t       | pointer to @ref object_t

*/
struct value_t {
public:
    ~value_t() = default;

    value_t() : type_(type::null) {};
    value_t(const boolean_t& b) : type_(type::boolean), boolean{b} {}
    value_t(const string_t& s) : type_(type::string), string{std::make_shared<string_t>(s)} {}
    value_t(const number_t& n) : type_(type::number_integer), number{n} {}
    value_t(const binary_t& b) : type_(type::binary), binary{std::make_shared<binary_t>(b)} {}

    json_string to_json() const {
        std::string ret;    ///< return value of type::array and type::object
        bool first = true;  ///< decide wether to add comma in return value of type::array and type::object
        switch (type_) {
        case detail::type::null:
            return "null";
        case detail::type::boolean:
            return boolean ? "true" : "false";
        case detail::type::string:
            return "\"" + *string + "\"";
        case detail::type::number_integer:
            return std::to_string(number.number_integer);
        case detail::type::number_unsigned:
            return std::to_string(number.number_unsigned);
        case detail::type::number_float:
            return std::to_string(number.number_float);
        case detail::type::binary:
            return sw::base64_encode(*binary);
        case detail::type::array:
            ret = "[";
            for (const auto& v : *array) {
                if (!first) ret += ", ";
                ret += v.to_json();
                first = false;
            }
            ret += "]";
            return ret;
        case detail::type::object:
            ret = "{";
            for (const auto& [k, v] : std::get<std::unordered_map<detail::string_t, value> >(*object)) {
                if (!first) ret += ", ";
                ret += "{\"" + k + "\": " + v.to_json() + "}";
                first = false;
            }
            ret += "}";
            return ret;
        case detail::type::ordered_object:
            ret = "{";
            for (const auto& [k, v] : std::get<std::map<detail::string_t, value> >(*object)) {
                if (!first) ret += ", ";
                ret += "{\"" + k + "\": " + v.to_json() + "}";
                first = false;
            }
            ret += "}";
            return ret;
        case detail::type::discarded:
            return "";
        }
    }

    union {
        boolean_t boolean;
        std::shared_ptr<string_t> string;
        number_t number;
        std::shared_ptr<binary_t> binary;
        std::shared_ptr<array_t> array;
        std::shared_ptr<object_t> object;
    };
    type type_; 
};

};

};
