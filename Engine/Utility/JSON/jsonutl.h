#pragma once

#include <nlohmann/json.hpp>

namespace utl
{
    namespace json
    {
        void try_assign(const nlohmann::json& _j, const std::string& _key, auto& _outval)
        {
            auto itr = _j.find(_key);
            if (itr == _j.end()) _outval = {};
            else itr->get_to(_outval);
        }

        void try_assign(
            const nlohmann::json& _j,
            const std::string& _key,
            auto&& _func,
            auto& _outval
        )
        {
            auto itr = _j.find(_key);
            if (itr == _j.end()) _outval = {};
            else _func(itr.value(), _outval);
        }
    }
}