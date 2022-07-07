

#pragma once

#include <memory>
#include <ostream>
#include <sstream>
#include <vector>

#include <lgg/logger_callback.h>
#include <lgg/logger_command.h>
#include <lgg/logger_concepts.h>
#include <lgg/logger_exception.h>
#include <lgg/logger_filter.h>
#include <lgg/logger_options.h>
#include <lgg/logger_ostream.h>
#include <lgg/logger_record.h>

namespace lgg
{
    template<
        is_logger_record t_record,
        is_logger_filter t_filter = logger_filter<>,
        typename... t_options
    >
    class logger
    {
    public:
        
        constexpr static auto gp_buffered = std::disjunction<
            std::is_same<
                option::buffered,
                t_options
            >...
        >::value;
    
    public:
        
        using type = logger;
        
        using record_type = t_record;
        
        using filter_type = t_filter;
        
        using buffer_type = std::stringstream;
        
        using ostreams_type = std::vector<
            std::shared_ptr<
                logger_ostream_basic
            >
        >;
        
        using cb_push_pre_type = logger_callback<
            void,
            record_type&
        >;
        
        using cb_push_post_type = logger_callback<
            void,
            record_type&
        >;
    
    private:
        
        record_type m_record;
        
        buffer_type m_buffer;
        
        ostreams_type m_ostreams;
        
        cb_push_pre_type m_cb_push_pre;
        
        cb_push_post_type m_cb_push_post;
    
    public:
        
        logger() = delete;
        
        logger(type&& obj) = default;
        
        logger(const type& obj) = delete;
        
        ~logger()
        {
            try
            {
                push();
            }
            catch (...)
            {
                // ignore
            }
        }
    
    public:
        
        logger(
            ostreams_type a_ostreams,
            cb_push_pre_type a_cb_push_pre,
            cb_push_post_type a_cb_push_post
        ) :
            m_ostreams(std::move(a_ostreams)),
            m_cb_push_pre(std::move(a_cb_push_pre)),
            m_cb_push_post(std::move(a_cb_push_post))
        {
            m_cb_push_post(
                m_record
            );
        }
        
        template<
            typename... t_record_args
        >
        logger(
            ostreams_type a_ostreams,
            cb_push_pre_type a_cb_push_pre,
            cb_push_post_type a_cb_push_post,
            t_record_args... a_record_args
        )
        requires(
            is_logger_record_constructible<
                record_type,
                t_record_args...
            >
        ):
            m_ostreams(std::move(a_ostreams)),
            m_cb_push_pre(std::move(a_cb_push_pre)),
            m_cb_push_post(std::move(a_cb_push_post)),
            m_record(std::forward<t_record_args>(a_record_args)...)
        {
            m_cb_push_post(
                m_record
            );
        }
    
    private:
        
        auto
        push() -> void
        {
            if (m_record.skip())
            {
                return;
            }
            
            m_cb_push_pre(
                m_record
            );
            
            if (filter_type::of(m_record))
            {
                if constexpr(gp_buffered)
                {
                    m_buffer << m_record;
                    
                    auto view = std::string_view{
                        m_buffer.view().data(),
                        static_cast<size_t>(m_buffer.tellp())
                    };
                    
                    for (auto& os: m_ostreams)
                    {
                        os->write(
                            view
                        );
                    }
                    
                    m_buffer.clear();
                    m_buffer.seekp(0);
                }
                else
                {
                    for (auto& os: m_ostreams)
                    {
                        os->write(
                            m_record
                        );
                    }
                }
            }
            
            m_cb_push_post(
                m_record
            );
            
            m_record.clear();
        }
    
    public:
        
        template<
            typename t_arg
        >
        friend auto
        operator <<(
            type& a_logger,
            const t_arg& a_arg
        ) -> type&
        {
            a_logger.m_record << a_arg;
            
            return a_logger;
        }
        
        friend auto
        operator <<(
            type& a_logger,
            command::push _ignore
        ) -> void
        {
            a_logger.push();
        }
    };
}
