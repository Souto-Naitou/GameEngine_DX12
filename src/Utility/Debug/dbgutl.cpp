#include "dbgutl.h"
#include <Utility/String/strutl.h>

std::string utl::debug::generate_name_default(void* _ptr)
{
    std::string result = "unnamed##";
    result += utl::string::to_string(_ptr);
    return result;
}

std::string utl::debug::generate_name(const std::string& _name, void* _ptr)
{
    std::string result = _name + "##";
    result += utl::string::to_string(_ptr);
    return result;
}
