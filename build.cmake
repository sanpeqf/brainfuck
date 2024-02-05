# SPDX-License-Identifier: GPL-2.0-or-later */
#
# Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
#

file(GLOB_RECURSE BRAINFUCK_HEADER
    ${PROJECT_SOURCE_DIR}/*.h
)

set(BRAINFUCK_SOURCE
    ${BFDEV_SOURCE}
    ${CMAKE_CURRENT_LIST_DIR}/brainfuck.c
)
