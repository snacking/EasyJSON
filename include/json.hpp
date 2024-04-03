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
// type
#include "./detail/type.hpp"
// serializer
#include "./detail/output/serializer.hpp"
// parser
#include "./detail/input/parser.hpp"

namespace sw {

class json {
public:

      ///< wrapper function for construct a json object with json string
    template <typename _Fn>
    static json parse(const detail::json_string& s, _Fn fn) {
        json = fn(s);
    }
private:
    detail::object_t content_;
};

};
