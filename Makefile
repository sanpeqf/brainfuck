# SPDX-License-Identifier: GPL-2.0-or-later */
#
# Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
#

brainfuck: brainfuck.c
	gcc -O2 -Wall -Werror -o ./brainfuck ./brainfuck.c -lbfdev

clean:
	rm -rf ./brainfuck
