/***
* ==++==
* By Lisw
* 
* ==--==
* =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
* Generator
*
* Generate table for base64 encode/decoder
* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
***/

// std::cout
#include <iostream>
// std::setw
#include <iomanip>

int main () {
    std::cout << "{";
    for (int i = 0; i < 256; ++i) {
        if (i % 16 == 0) {
            std::cout << std::endl;
        }
        std::cout << std::setw(3);
        switch (i) {
        case '+':
        case '-':
            std::cout << "62, ";
            break;
        case '/':
        case '_':
            std::cout << "63, ";
            break;
        default:
            if (i >= 48 && i <= 57) {
                std::cout << i - 48 + 52 << ", ";
            } else if (i >= 65 && i <= 90) {
                std::cout << i - 65 + 10 << ", ";
            } else if (i >= 97 && i <= 122) {
                std::cout << i - 97 + 36 << ", ";
            } else {
                std::cout << "255, ";
            }
            break;
        }
    }
    std::cout << "}" << std::endl;
    return 0;
}