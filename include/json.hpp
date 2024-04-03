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
// parser
#include "./detail/parser.hpp"

namespace sw {

class json {
public:

      ///< wrapper function for construct a json object with json string
    template <typename _Fn>
    static json parse(const json_string& s, _Fn fn) {
        json = fn(s);
    }
private:
    object_t content_;
};

};
