

#pragma once

#include <concepts>
#include <type_traits>
#include <utility>

#include <lgg/logger_record.h>

namespace lgg
{
    template<
        typename t_record
    > concept is_logger_record =
    requires(
        t_record& a_record,
        std::ostream& a_ostream
    )
    {
        requires std::is_default_constructible<
            t_record
        >::value;
        requires std::is_base_of_v<
            logger_record_basic,
            t_record
        >;
        {
        a_record.clear()
        } -> std::same_as<void>;
        {
        a_record.skip()
        } -> std::same_as<bool>;
        {
        a_record.dump(a_ostream)
        } -> std::same_as<void>;
        {
        a_ostream << a_record
        };
        {
        a_record << 1 << ' ' << " "
        };
    };
    
    template<
        typename t_record,
        typename... t_record_args
    > concept is_logger_record_constructible =
    requires
    {
        requires is_logger_record<t_record>;
        requires std::is_constructible<
            t_record,
            t_record_args...
        >::value;
    };
    
    /*
     * TODO
     */
    template<
        typename t_filter
    > concept is_logger_filter = true;
}
