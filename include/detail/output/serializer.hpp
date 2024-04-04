/***
* ==++==
* By Lisw
* 
* ==--==
* =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
* Parser
*
* Main public header file for serialize
* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
***/

#pragma once

// std::enable_if, std::true_type, std::false_type
#include <type_traits>
// static_assert
#include <cassert>
// std::string
#include <string>


namespace sw {

namespace detail {

using json_string = std::string;
  
template<typename, typename = std::void_t<>>  
struct has_to_json : std::false_type {};  
  
template<typename _Tp>  
struct has_to_json<_Tp, std::void_t<decltype(std::declval<_Tp>().to_json())>> : std::true_type {};

template <typename _Tp>  
json_string serialize(_Tp v) {
    static_assert(has_to_json<_Tp>::value, "Type does not have a to_json member function");
    return v.to_json();  
}

};

};
