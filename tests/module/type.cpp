/***
* ==++==
* By Lisw
* 
* ==--==
* =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
* type.cpp
*
* Test file for type.hpp
* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
***/

#include "../../include/detail/type.hpp"
#include <iostream>

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

int main() {
    sw::detail::value_t o(sw::detail::type::object);
    auto& object = o.get<sw::detail::object_t>();
    object.emplace("name", sw::detail::value_t("lisw"));
    object.emplace("male", sw::detail::value_t(true));
    object.emplace("age", sw::detail::value_t((std::int64_t)20));
    sw::detail::value_t a(sw::detail::type::array);
    auto& array = a.get<sw::detail::array_t>();
    array.emplace_back(sw::detail::value_t("coconut"));
    array.emplace_back(sw::detail::value_t("chocolate"));
    array.emplace_back(sw::detail::value_t("dog"));
    array.emplace_back(sw::detail::value_t((std::int64_t)7));
    object.emplace("favorite", a);


    std::cout << "Serialize: " << sw::detail::serialize(o) << std::endl;
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG); 
    _CrtDumpMemoryLeaks();
    return 0;
}

