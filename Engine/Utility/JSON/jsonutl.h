#pragma once

#include <nlohmann/json.hpp>

namespace utl
{
    namespace json
    {
        template <typename T>
        void try_assign(const nlohmann::json& _j, const std::string& _key, T& _outval)
        {
            auto itr = _j.find(_key);
            if (itr == _j.end()) _outval = T();
            else itr->get_to(_outval);
        }
    }
}