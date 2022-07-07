

#pragma once

#include <exception>

namespace lgg
{
    class logger_exception
        :
            public std::exception
    {
    public:
        
        using std::exception::exception;
    };
}
