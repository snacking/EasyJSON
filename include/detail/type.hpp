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

namespace sw {

namespace detail {

union value_t;
class value;

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
    object,           ///< object (set of key/value pairs)
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

union value_t {
    std::shared_ptr<string_t> string;
    boolean_t boolean;
    number_t number;
    std::shared_ptr<binary_t> binary;
    std::shared_ptr<array_t> array;
    std::shared_ptr<object_t> object;

    value_t() = default;
    ~value_t() = default;
    value_t(boolean_t v) noexcept { boolean = v; }
};

class value {
public:
    bool operator < (const value& i) {
        return order_ == i.order_ ?
            type_ < i.type_ : order_ < i.order_;
    }

    json_string to_json() const {
        std::string ret;    ///< return value of type::array and type::object
        bool first = true;  ///< decide wether to add comma in return value of type::array and type::object
        switch (type_) {
        case type::null:
            return "null";
        case type::boolean:
            return value_.boolean ? "true" : "false";
        case type::string:
            return "\"" + *value_.string + "\"";
        case type::number_integer:
            return std::to_string(value_.number.number_integer);
        case type::number_unsigned:
            return std::to_string(value_.number.number_unsigned);
        case type::number_float:
            return std::to_string(value_.number.number_float);
        case type::binary:
        case type::array:
            ret = "[";
            for (const auto& v : *value_.array) {
                if (!first) ret += ", ";
                ret += v.to_json();
                first = false;
            }
            ret += "]";
            return ret;
        case type::object:
            ret = "{";
            for (const auto& [k, v] : (ordered_ ? std::get<std::map<string_t, value> >(*value_.object) : std::get<std::unordered_map<string_t, value> >(*value_.object))) {
                if (!first) ret += ", ";
                ret += "{";
                ret += k;
                ret += ": ";
                ret += v.to_json();
                ret += "}";
                first = false;
            }
            ret += "}";
            return ret;
        case type::discarded:
        default:
        }
    }
private:
    value_t value_;     ///< value inside
    string_t string_;   ///< raw string (allow partly update json string)
    type type_;         ///< type of value
    int order_;         ///< priority of value (allow serialize in original order)
    bool ordered_;      ///< indicate wether the object map inside is ordered, the convert from ordered to unordered is NOT reversible
};

};

};
