cmake_minimum_required(VERSION 3.21)
project(
        lgg
        VERSION 0.0.1
)

include(GNUInstallDirs)

# Configuration
set(CMAKE_CXX_STANDARD 20)

# Options
option(LGG_EXAMPLES "Enable examples." OFF)

# Variables
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

# Library
add_library(
        ${PROJECT_NAME} INTERFACE
)

target_include_directories(
        ${PROJECT_NAME}
        INTERFACE
        $<BUILD_INTERFACE:${INCLUDE_DIR}>
        $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
)

# Installation
install(
        TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}-config
)

install(
        DIRECTORY ${INCLUDE_DIR}/${PROJECT_NAME}/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
)

install(
        EXPORT ${PROJECT_NAME}-config
        DESTINATION "${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}/cmake"
)

add_subdirectory(examples)
