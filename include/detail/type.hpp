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
// std::int64_t, std::uint64_t, std::uint8_t
#include <cstdint>
// std::optional
#include <optional>
// std::invalid_argument
#include <stdexcept>
// serialze
#include "./output/serializer.hpp"
// base64
#include "./conversions/base64.hpp"

namespace sw {

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

struct value_t;

using boolean_t = bool;
using string_t = std::string;
using number_t = union {
    std::int64_t number_integer;
    std::uint64_t number_unsigned;
    double number_float;
};
using binary_t = std::vector<std::uint8_t>;
using array_t = std::vector<value_t>;
using object_t = std::unordered_map<string_t, value_t>;
using ordered_object_t = std::map<string_t, value_t>;

template <typename _Tp>
_Tp make() {
    return _Tp();
}

template <>
boolean_t make<boolean_t>() {
    return false;
}

template <>
string_t make<string_t>() {
    return std::string();
}

template <>
number_t make<number_t>() {
    return {0};
}

template <>
binary_t make<binary_t>() {
    return {};
}

template <>
array_t make<array_t>() {
    return {};
}

template <>
object_t make<object_t>() {
    return object_t();
}

template <>
ordered_object_t make<ordered_object_t>() {
    return ordered_object_t();
}



/*

JSON type       | valueT type        | used type
----------------| -------------------|------------------------
null            | null               | *no value is stored*
boolean         | boolean_t          | @ref boolean_t
string          | string_t           | pointer to @ref string_t
number          | number_t           | @ref number_t
binary          | binary_t           | pointer to @ref binary_t
array           | array_t            | pointer to @ref array_t
object          | object_t           | pointer to @ref object_t
ordered_object  | ordered_object_t   | pointer to @ref ordered_object_t

*/
struct value_t {
public:
    value_t() : type_(type::null) {};
    value_t(const boolean_t& b) : type_(type::boolean), boolean{b} {}
    value_t(const string_t& s) : type_(type::string), string{std::make_shared<string_t>(s)} {}
    value_t(const std::int64_t& n) : type_(type::number_integer) { number.number_integer = n; }
    value_t(const std::uint64_t& n) : type_(type::number_unsigned) { number.number_unsigned = n; }
    value_t(const double& n) : type_(type::number_float) { number.number_float = n; }
    value_t(const binary_t& b) : type_(type::binary), binary{std::make_shared<binary_t>(b)} {}
    value_t(const array_t& a) : type_(type::array), array{std::make_shared<array_t>(a) } {}
    value_t(const object_t& o) : type_(type::object), object{std::make_shared<object_t>(
                                    std::unordered_map<string_t, value_t>(o)) } {}
    value_t(const ordered_object_t& o) : type_(type::ordered_object), ordered_object{std::make_shared<ordered_object_t>(
                                            std::map<string_t, value_t>(o)) } {}
    value_t(const value_t& t) : type_(t.type_) {
        switch (type_) {
        case type::boolean:
            boolean = t.boolean;
            break;
        case type::string:
            string = t.string;
            break;
        case type::number_integer:
            number.number_integer = t.number.number_integer;
            break;
        case type::number_unsigned:
            number.number_unsigned = t.number.number_unsigned;
            break;
        case type::number_float:
            number.number_float = t.number.number_float;
            break;
        case type::binary:
            binary = t.binary;
            break;
        case type::array:
            array = t.array;
            break;
        case type::object:
            object = t.object;
            break;
        case type::ordered_object:
            ordered_object = t.ordered_object;
            break;
        }
    }

    value_t(const type& t) : type_(t) {
        switch (t) {
        case type::boolean:
            boolean = make<boolean_t>();
            break;
        case type::string:
            string = std::make_shared<string_t>(make<string_t>());
            break;
        case type::number_integer:
            number.number_integer = make<number_t>().number_integer;
            break;
        case type::number_unsigned:
            number.number_unsigned = make<number_t>().number_unsigned;
            break;
        case type::number_float:
            number.number_float = make<number_t>().number_float;
            break;
        case type::binary:
            binary = std::make_shared<binary_t>(make<binary_t>());
            break;
        case type::array:
            array = std::make_shared<array_t>(make<array_t>());
            break;
        case type::object:
            object = std::make_shared<object_t>(make<object_t>());
            break;
        case type::ordered_object:
            ordered_object = std::make_shared<ordered_object_t>(make<ordered_object_t>());
            break;
        }
    }

    ~value_t() {
        switch (type_) {
        case type::string:
            string.~shared_ptr<string_t>();
            break;
        case type::binary:
            binary.~shared_ptr<binary_t>();
            break;
        case type::array:
            array.~shared_ptr<array_t>();
            break;
        case type::object:
            object.~shared_ptr<object_t>();
            break;
        case type::ordered_object:
            ordered_object.~shared_ptr<ordered_object_t>();
            break;
        }
    }

    template <typename _Tp>
    _Tp& get() {
        switch (type_) {
        case type::boolean:
            if constexpr(std::is_same<boolean_t, _Tp>::value)
                return boolean;
        case type::string:
            if constexpr(std::is_same<string_t, _Tp>::value)
                return *string;
        case type::number_integer:
            if constexpr(std::is_same<std::int64_t, _Tp>::value)
                return number.number_integer;
        case type::number_unsigned:
            if constexpr(std::is_same<std::uint64_t, _Tp>::value)
                return number.number_unsigned;
        case type::number_float:
            if constexpr(std::is_same<double, _Tp>::value)
                return number.number_float;
        case type::binary:
            if constexpr(std::is_same<binary_t, _Tp>::value)
                return *binary;
        case type::array:
            if constexpr(std::is_same<array_t, _Tp>::value)
                return *array;
        case type::object:
            if constexpr(std::is_same<object_t, _Tp>::value)
                return *object;
        case type::ordered_object:
            if constexpr(std::is_same<ordered_object_t, _Tp>::value)
                return *ordered_object;
        }
        throw std::invalid_argument("Type not supported for get function");
    }

    json_string to_json() const {
        json_string ret;    ///< return value of type::array and type::object
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
            return sw::base64_encode(*binary, sizeof(*binary) * 8);
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
            for (const auto& [k, v] : *object) {
                if (!first) ret += ", ";
                ret += "{\"" + k + "\": " + v.to_json() + "}";
                first = false;
            }
            ret += "}";
            return ret;
        case detail::type::ordered_object:
            ret = "{";
            for (const auto& [k, v] : *ordered_object) {
                if (!first) ret += ", ";
                ret += "{\"" + k + "\": " + v.to_json() + "}";
                first = false;
            }
            ret += "}";
            return ret;
        }
        throw std::invalid_argument("Type not supported for get function");
    }

    union {
        boolean_t boolean;
        std::shared_ptr<string_t> string;
        number_t number;
        std::shared_ptr<binary_t> binary;
        std::shared_ptr<array_t> array;
        std::shared_ptr<object_t> object;
        std::shared_ptr<ordered_object_t> ordered_object;
    };
    type type_; 
};

};

};
