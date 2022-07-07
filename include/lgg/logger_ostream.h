

#pragma once

#include <mutex>
#include <ostream>
#include <utility>

#include <lgg/logger.h>
#include <lgg/logger_record.h>

namespace lgg
{
    class logger_ostream_basic
    {
    public:
        
        using type = logger_ostream_basic;
        
        using record_type = logger_record_basic;
    
    public:
        
        logger_ostream_basic() = default;
        
        logger_ostream_basic(const type& obj) = delete;
        
        logger_ostream_basic(type&& obj) = default;
        
        virtual ~logger_ostream_basic() = default;
    
    public:
        
        virtual auto
        write(
            const record_type& a_record
        ) -> type& = 0;
        
        virtual auto
        write(
            std::istream& a_istream
        ) -> type& = 0;
    };
    
    template<
        typename t_ostream
    >
    class logger_ostream;
    
    template<>
    class logger_ostream<
        std::ostream
    >
        :
            public logger_ostream_basic
    {
    public:
        
        using type = logger_ostream;
        
        using stream_type = std::ostream;
        
        using cb_write_pre_type = logger_callback<
            void,
            stream_type&
        >;
    
    private:
        
        cb_write_pre_type m_cb_write_pre;
        
        stream_type& m_stream;
        
        std::mutex m_mutex;
    
    public:
        
        logger_ostream(
            stream_type& a_stream,
            cb_write_pre_type a_cb_write_pre = cb_write_pre_type{
                [](stream_type& _) -> void
                {
                }
            }
        ) :
            m_stream(a_stream),
            m_cb_write_pre(std::move(a_cb_write_pre))
        {
        }
        
        logger_ostream(const type& obj) = delete;
        
        logger_ostream(type&& obj) = delete;
        
        ~logger_ostream() override = default;
    
    public:
        
        auto
        write(
            const record_type& a_record
        ) -> type& override
        {
            {
                std::unique_lock lock{m_mutex};
                
                m_cb_write_pre(
                    m_stream
                );
                
                a_record.write(
                    m_stream
                );
            }
            
            return *this;
        }
        
        auto
        write(
            std::istream& a_istream
        ) -> type& override
        {
            auto start_g = a_istream.tellg();
            {
                std::unique_lock lock{m_mutex};
                
                m_cb_write_pre(
                    m_stream
                );
                
                m_stream << a_istream.rdbuf();
            }
            a_istream.seekg(start_g);
            
            return *this;
        }
    };
    
    logger_ostream(
        std::ostream&
    ) -> logger_ostream<std::ostream>;
    
    logger_ostream(
        std::ostream&,
        logger_callback<
            void,
            std::ostream&
        >
    ) -> logger_ostream<std::ostream>;
}
