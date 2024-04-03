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
// std::any
#include <any>
// std::optional
#include <optional>

namespace sw {

using json_string = std::string;

union value_t;
class inner;

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
    object,           ///< object (unordered set of name/value pairs)
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
using array_t = std::vector<value_t>;
using object_t = union {
    std::map<string_t, inner> ordered_map;
    std::unordered_map<string_t, inner> unordered_map;
};

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
    value_t(boolean_t v) noexcept { boolean = v; }

    json_string serialize(type type_) {
        json_string str = "";
        switch (type_)
        {
        case type::null:
            str += "null";
            break;
        case type::boolean:
            str += "null";
            break;
        default:
            break;
        }
    } 
};

class inner {
public:
    bool operator < (const inner& i) {
        return order_ == i.order_ ?
            type_ < i.type_ : order_ < i.order_;
    }

    json_string serialize() {
        return value_.serialize(type_);
    }
private:
    value_t value_;     ///< real value
    string_t string_;   ///< raw string (allow partly update json string)
    type type_;         ///< type of value
    int order_;      ///< priority of value (allow serialize in original order)
};

};
