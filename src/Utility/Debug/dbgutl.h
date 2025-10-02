#pragma once
#include <string>

namespace utl
{
    namespace debug
    {
        std::string generate_name_default(void* _ptr);
        std::string generate_name(const std::string& _name, void* _ptr);
    }
}