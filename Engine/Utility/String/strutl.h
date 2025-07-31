#pragma once

#include <string>
#include <cstdio>
#include <cstdint>

std::string operator"" _s(const char* _str, size_t _len);

namespace utl::string
{
    template <typename T>
    inline std::string to_string(const T* _ptr)
    {
        char buffer[20];
        std::snprintf(buffer, sizeof(buffer), "%p", static_cast<const void*>(_ptr));
        return std::string(buffer);
    }

    uint32_t to_hash(const std::string& _str);

    std::string to_lower(const std::string& _str);
}