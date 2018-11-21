// Copyright (c) 2017 The TAL Authors.
// All rights reserved.

#ifndef BASE_URL_URL_UTIL_H_
#define BASE_URL_URL_UTIL_H_
#include "base/base_export.h"

namespace base {
namespace url {
inline unsigned char ToHex(unsigned char x) {
    return  x > 9 ? x + 55 : x + 48;
}

inline unsigned char FromHex(unsigned char x) {
    unsigned char y;
    if(x >= 'A' && x <= 'Z') y = x - 'A' + 10;
    else if(x >= 'a' && x <= 'z') y = x - 'a' + 10;
    else if(x >= '0' && x <= '9') y = x - '0';
    else return -1;
    return y;
}

inline std::string UrlEncode(const std::string& str) {
    std::string strTemp = "";
    size_t length = str.length();
    for(size_t i = 0; i < length; i++) {
        if(isalnum((unsigned char)str[i]) ||
                (str[i] == '-') ||
                (str[i] == '_') ||
                (str[i] == '.') ||
                (str[i] == '~'))
            strTemp += str[i];
        else if(str[i] == ' ')
            strTemp += "+";
        else {
            strTemp += '%';
            strTemp += ToHex((unsigned char)str[i] >> 4);
            strTemp += ToHex((unsigned char)str[i] % 16);
        }
    }
    return strTemp;
}

inline std::string UrlDecode(const std::string& str) {
    std::string strTemp = "";
    size_t length = str.length();
    for(size_t i = 0; i < length; i++) {
        if(str[i] == '+') strTemp += ' ';
        else if(str[i] == '%') {
            //::Assert(i + 2 < length);
            unsigned char high = FromHex((unsigned char)str[++i]);
            unsigned char low = FromHex((unsigned char)str[++i]);
            strTemp += high * 16 + low;
        } else strTemp += str[i];
    }
    return strTemp;
}
}
}

#endif  // BASE_URL_URL_UTIL_H_