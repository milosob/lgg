#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>

#include <string>
#include <utility>
#include <lgg/logger.h>

/**
 * Implement a custom logger levels.
 */
enum logger_level
    :
        uint32_t
{
    DEBUG = 0,
    INFO  = 1,
    WARN  = 2,
    ERROR = 3,
    FATAL = 4
};

auto
operator <<(
    std::ostream& a_ostream,
    logger_level a_level
) -> std::ostream&
{
    constexpr static std::array<
        std::string_view,
        6
    > logger_level_map{
        "DEBUG",
        "INFO",
        "WARN",
        "ERROR",
        "FATAL"
    };
    
    return a_ostream << logger_level_map[a_level];
}

/**
 * Implement a custom record type.
 */
class logger_record
    :
        public lgg::logger_record_basic
{
public:
    
    using type = logger_record;

private:
    
    using identifier_type = std::string;
    
    using buffer_type = std::stringstream;

private:
    
    identifier_type m_identifier;
    
    buffer_type m_buffer;

public:
    
    std::chrono::system_clock::time_point p_timestamp;
    
    logger_level p_level = INFO;

public:
    
    logger_record() = default;
    
    logger_record(type&& obj) = default;
    
    logger_record(const type& obj) = delete;
    
    ~logger_record() override = default;

public:
    
    logger_record(
        identifier_type a_identifier
    ) :
        m_identifier(std::move(a_identifier))
    {
    }

public:
    
    /**
     * Called always after pushing the log.
     */
    auto
    clear(
    ) -> void
    {
        m_buffer.seekg(0);
        m_buffer.seekp(0);
    }
    
    /**
     * Skip pushing the log when this returns true.
     */
    auto
    skip(
    ) -> bool
    {
        return m_buffer.tellp() == buffer_type::pos_type(0);
    }
    
    /**
     * Implement the interface write function that writes to the stream.
     */
    auto
    write(
        std::ostream& a_ostream
    ) const -> void override
    {
        auto time      = std::tm{};
        auto timestamp = std::chrono::system_clock::to_time_t(p_timestamp);
        
        localtime_r(
            &timestamp,
            &time
        );
        
        a_ostream
            << std::put_time(
                &time,
                "%F %T "
            )
            << "[id="
            << m_identifier
            << "] "
            << "[level="
            << p_level
            << "]: "
            << m_buffer.view()
            /*
             * Flush by default to avoid losing logs.
             */
            << std::endl;
    }

public:
    
    /**
     * Implement how does the record handle writing to it.
     * It is possible to provide more accurate specializations for setter like behavior.
     */
    template<
        typename t_arg
    >
    friend auto
    operator <<(
        type& a_record,
        const t_arg& a_arg
    ) -> type&
    {
        a_record.m_buffer << a_arg;
        
        return a_record;
    }
    
    /**
     * Implement ostream << operator, for buffering purposes.
     */
    friend auto
    operator <<(
        std::ostream& a_stream,
        const type& a_record
    ) -> std::ostream&
    {
        a_record.write(
            a_stream
        );
        
        return a_stream;
    }
};

/**
 * Define a type to group filters.
 */
struct logger_filter_group
{
};

/**
 * Provide a filter specialization of record in the selected group.
 */
template<>
template<>
auto
lgg::logger_filter<
    logger_filter_group
>::of<logger_record>(
    const logger_record& a_record
) -> bool
{
    return a_record.p_level >= DEBUG;
}

/**
 * Alias types for easy usage.
 */
using logger = lgg::logger<
    logger_record,
    /**
     * Use defined logger or use predefined one:
     * lgg::logger_filter<> that accepts everything.
     */
    lgg::logger_filter<logger_filter_group>,
    /**
     * If more than one output is given, consider enabling buffered logging.
     */
    lgg::option::buffered
>;

int
main(
    int a_argc,
    char** a_argv
)
{
    /**
     * Provide the example with optional path argument to also write to the file log.
     */
    std::ofstream log_file;
    
    /**
     * These are logging ostreams.
     * A record will be written to each of them.
     * They can be shared across many loggers.
     *
     * The library ostream specializations by default are thread safe using mutexes.
     * It is possible to use a custom ostream logic by providing the specialization for lgg::logger_ostream.
     */
    logger::ostreams_type lgos{
        std::make_shared<lgg::logger_ostream<std::ostream>>(
            std::cerr
        ),
        std::make_shared<lgg::logger_ostream<std::ostream>>(
            std::cout
        )
    };
    
    if (a_argc >= 2)
    {
        log_file.open(
            a_argv[1],
            std::ios_base::out | std::ios_base::app
        );
        
        if (log_file.is_open())
        {
            lgos.emplace_back(
                std::make_shared<lgg::logger_ostream<std::ostream>>(
                    log_file
                )
            );
        }
    }
    
    logger lg{
        {
            lgos
        },
        /**
         * This is invoked before every push.
         */
        logger::cb_push_pre_type{
            [](logger::record_type& a_record)
            {
                a_record.p_timestamp = std::chrono::system_clock::now();
            }
        },
        /**
         * This is invoked after every push.
         */
        logger::cb_push_post_type{
            [](logger::record_type& a_record)
            {
                a_record.p_level = DEBUG;
            }
        },
        /**
         * From here, the arguments are passed to the record constructor.
         * In this specific case, the identifier is passed.
         */
        "logger_example"
    };
    
    lg << "message_1_part_0 " << "message_1_part_1 " << "message_1_part_2" << lgg::push;
    lg << "message_2_part_0 " << "message_2_part_1 " << "message_2_part_2" << lgg::push;
    lg << "message_3_part_0 " << "message_3_part_1 " << "message_3_part_2 ";
    lg << "message_3_part_3 " << "message_3_part_4 " << "message_3_part_5" << lgg::push;
    
    if (log_file.is_open())
    {
        log_file.close();
    }
    
    return 0;
}
