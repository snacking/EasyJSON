/***
* ==++==
* By Lisw
* 
* ==--==
* =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
* Parser
*
* Main public header file for EasyJSON
* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
***/

#pragma once
// std::variant::get
#include <variant>
// type
#include "./detail/type.hpp"
// serializer
#include "./detail/output/serializer.hpp"
// parser
#include "./detail/input/parser.hpp"

namespace sw {

class value {
public:
    value() = default;
    value(detail::boolean_t b) : value_.boolean(b), type_(detail::type::boolean), order_(-1) {}
    bool operator < (const value& i) {
        return order_ == i.order_ ?
            type_ < i.type_ : order_ < i.order_;
    }
private:
    detail::value_t value_;     ///< value inside
    detail::string_t string_;   ///< raw string (allow partly update json string)
    int order_;                 ///< order of value
};

class json;

template<typename, typename = std::void_t<>>  
struct has_from_json : std::false_type {};  

template<typename _Tp>  
struct has_from_json<_Tp, std::void_t<decltype(_Tp::from_json(std::declval<const json&>()))>> : std::true_type {};

class json {
public:

    ///< wrapper function for construct a json object with json string
    template <typename _Fn>
    static json parse(const detail::json_string& s, _Fn fn) {
        json = fn(s);
    }

    ///< if type _Tp has a from_json() function, 
    template<typename _Tp, std::enable_if_t<has_from_json<_Tp>::value, int> = 0>  
    _Tp& operator = (const _Tp& other) {  
        *this = from_json<_Tp>(*this);  
        return *this;  
    }  

private:
    template<typename _Tp, std::enable_if_t<has_from_json<_Tp>::value, int> = 0>  
    static _Tp from_json(const json& json) {  
        return _Tp::from_json(json);  
    }  

    template<typename _Tp, std::enable_if_t<!has_from_json<_Tp>::value, int> = 0>  
    static _Tp from_json(const json& json) {  
        static_assert(std::is_same<_Tp, void>::value, "Type does not have a from_json static member function");  
        return _Tp(); 
    } 

    detail::object_t content_;
};

};
