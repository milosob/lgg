project(
        ${CMAKE_PROJECT_NAME}_examples
        VERSION ${CMAKE_PROJECT_VERSION}
)

if (NOT ${LGG_EXAMPLES})
    return()
endif ()

add_executable(
        ex_custom_record_logger ex_custom_record_logger.cpp
)

target_link_libraries(
        ex_custom_record_logger
        PRIVATE
        lgg
)
