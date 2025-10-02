#include "fsutl.h"

#include <Utility/String/strutl.h>

std::filesystem::path utl::filesystem::to_lower(const std::filesystem::path& _path)
{
    std::filesystem::path result = {};

    result = string::to_lower(_path.string());

    return result;
}

std::filesystem::path utl::filesystem::get_parent_path(const std::filesystem::path& _path)
{
    std::filesystem::path result = {};
    if (_path.has_parent_path())
    {
        result = _path.parent_path();
    }
    else
    {
        result = _path;
    }
    return result;
}

std::string utl::filesystem::get_parent_path_string(const std::filesystem::path& _path)
{
    return get_parent_path(_path).string();
}
