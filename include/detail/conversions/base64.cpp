/***
* ==++==
* By Lisw
* 
* ==--==
* =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
* Parser
*
* Main public header file for Base64 encoding and decoding
* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
***/

#pragma once

// std::vector
#include <vector>
// std::uint8_t
#include <cstdint>
// std::string
#include <string>

using string_t = std::string;
using binary_t = std::vector<uint8_t>;

constexpr char base64_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

string_t base64_encode(const binary_t& input) {
    string_t result;
    int padding = 0;

    for (size_t i = 0; i < input.size(); i += 3) {
        uint32_t octet_a = i < input.size() ? input[i] : 0;
        uint32_t octet_b = i + 1 < input.size() ? input[i + 1] : 0;
        uint32_t octet_c = i + 2 < input.size() ? input[i + 2] : 0;

        uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;

        result.push_back(base64_chars[(triple >> 3 * 6) & 0x3F]);
        result.push_back(base64_chars[(triple >> 2 * 6) & 0x3F]);
        if (i + 1 < input.size()) {
            result.push_back(base64_chars[(triple >> 1 * 6) & 0x3F]);
        } else {
            padding++;
            result.push_back('=');
        }
        if (i + 2 < input.size()) {
            result.push_back(base64_chars[(triple >> 0 * 6) & 0x3F]);
        } else {
            padding++;
            result.push_back('=');
        }
    }

    result.resize(result.size() - padding);
    return result;
}