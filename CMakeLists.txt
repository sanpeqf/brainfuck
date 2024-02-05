# SPDX-License-Identifier: GPL-2.0-or-later */
#
# Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
#

cmake_minimum_required(VERSION 3.12)
project(brainfuck VERSION 1.0 LANGUAGES C)

include(GNUInstallDirs)
include(CheckIncludeFiles)

option(BRAINFUCK_CLI "Enable the command line interface" ON)
option(BRAINFUCK_INSTALL_TESTSUITE "Install the testsuite" OFF)

add_compile_options(
    -std=gnu11
    -Wall
    -Wextra
    -Werror
)

find_package(bfdev REQUIRED)
include_directories(${PROJECT_SOURCE_DIR})
include(${PROJECT_SOURCE_DIR}/build.cmake)

set(BRAINFUCK_LIBRARY
    ${BRAINFUCK_SOURCE}
    ${BRAINFUCK_HEADER}
)

add_library(brainfuck_object OBJECT ${BRAINFUCK_LIBRARY})
target_link_libraries(brainfuck_object bfdev)

if(${CMAKE_PROJECT_NAME} STREQUAL "brainfuck")
    add_library(brainfuck_static STATIC ${BRAINFUCK_LIBRARY})
    add_library(brainfuck_shared SHARED ${BRAINFUCK_LIBRARY})

    target_link_libraries(brainfuck_static bfdev)
    target_link_libraries(brainfuck_shared bfdev)

    set_target_properties(brainfuck_static
        PROPERTIES
        OUTPUT_NAME ${PROJECT_NAME}
        LINKER_LANGUAGE C
    )

    set_target_properties(brainfuck_shared
        PROPERTIES
        OUTPUT_NAME ${PROJECT_NAME}
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION_MAJOR}
        LINKER_LANGUAGE C
        MACOSX_RPATH ON
    )

    if(BRAINFUCK_CLI)
        add_executable(brainfuck main.c)
        target_link_libraries(brainfuck brainfuck_object)
    endif()

    install(FILES
        ${PROJECT_BINARY_DIR}/brainfuck
        PERMISSIONS
        OWNER_READ OWNER_WRITE OWNER_EXECUTE
        GROUP_READ GROUP_EXECUTE
        WORLD_READ WORLD_EXECUTE
        DESTINATION ${CMAKE_INSTALL_BINDIR}
    )

    install(DIRECTORY
        ${PROJECT_SOURCE_DIR}/testsuite
        DESTINATION ${CMAKE_INSTALL_DOCDIR}
    )

    install(FILES
        ${PROJECT_SOURCE_DIR}/README.md
        ${PROJECT_SOURCE_DIR}/COPYING
        DESTINATION ${CMAKE_INSTALL_DOCDIR}
    )

    install(TARGETS
        brainfuck_static brainfuck_shared
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )
endif()
