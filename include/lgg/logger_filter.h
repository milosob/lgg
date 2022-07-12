

#pragma once

#include <lgg/logger_concepts.h>

namespace lgg
{
    namespace filter
    {
        namespace group
        {
            struct pass
            {
            };
        }
    }
    
    template<
        typename t_filter = filter::group::pass
    >
    class logger_filter
    {
    public:
        
        using type = logger_filter;
        
        using filter_type = t_filter;
    
    public:
        
        logger_filter() = delete;
        
        logger_filter(type&& obj) = delete;
        
        logger_filter(const type& obj) = delete;
        
        ~logger_filter() = delete;
    
    public:
        
        template<
            is_logger_record t_record
        >
        static auto
        of(
            const t_record& a_record
        ) -> bool
        {
            return true;
        };
    };
}
