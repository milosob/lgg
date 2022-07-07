

#pragma once

#include <ostream>

namespace lgg
{
    class logger_record_basic
    {
    public:
        
        using type = logger_record_basic;
    
    public:
        
        logger_record_basic() = default;
        
        logger_record_basic(type&& obj) = default;
        
        logger_record_basic(const type& obj) = default;
        
        virtual ~logger_record_basic() = default;
    
    public:
        
        virtual auto
        write(
            std::ostream& a_ostream
        ) const -> void = 0;
    };
}
