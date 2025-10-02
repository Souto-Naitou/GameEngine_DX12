#include "strutl.h"
#include <algorithm>
#include <cctype>

uint32_t utl::string::to_hash(const std::string& _str)
{
    uint32_t hash = 0;
    for (char c : _str)
    {
        hash = (hash * 31) + static_cast<uint8_t>(c);
    }
    return hash;
}

std::string utl::string::to_lower(const std::string& _str)
{
    std::string lowerStr = _str;

    std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(),
        [](unsigned char c) -> char {
        return static_cast<char>(std::tolower(c));
    });

    return lowerStr;
}

std::string operator""_s(const char* _str, size_t _len)
{
    return std::string(_str, _len);
}
