

#pragma once

#include <functional>
#include <ostream>
#include <utility>

namespace lgg
{
    template<
        typename t_return,
        typename... t_args
    >
    class logger_callback
    {
    public:
        
        using type = logger_callback;
        
        using return_type = t_return;
        
        using callback_type = std::function<
            return_type(
                t_args...
            )
        >;
    
    private:
        
        callback_type m_callback;
    
    public:
        
        logger_callback() = default;
        
        logger_callback(type&& obj) = default;
        
        logger_callback(const type& obj) = default;
        
        ~logger_callback() = default;
    
    public:
        
        logger_callback(
            callback_type a_callback
        ) :
            m_callback(a_callback)
        {
        }
    
    public:
        
        auto
        operator ()(
            t_args... a_args
        ) -> return_type
        {
            return m_callback(
                std::forward<t_args>(a_args)...
            );
        }
    };
}
