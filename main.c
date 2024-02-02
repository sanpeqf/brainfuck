/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#define MODULE_NAME "brainfuck"
#define bfdev_log_fmt(fmt) MODULE_NAME ": " fmt

#include <stdio.h>
#include <bfdev.h>

extern int
brainfuck(const char *syntax, const char **endp);

int main(int argc, const char *argv[])
{
    const char *errinfo, *end;
    int index, retval;

    for (index = 1; index < argc; ++index) {
        retval = brainfuck(argv[index], &end);
        if (bfdev_likely(!retval))
            continue;

        bfdev_errname(retval, &errinfo);
        bfdev_log_err("execute error: %s\n", errinfo);
        return retval;
    }

    return 0;
}
