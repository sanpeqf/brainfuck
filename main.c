/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#define MODULE_NAME "brainfuck"
#define bfdev_log_fmt(fmt) MODULE_NAME ": " fmt

#include <stdio.h>
#include <unistd.h>
#include <err.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <bfdev.h>

extern int
brainfuck(const char *syntax, const char **endp);

static inline void
show_error(const char *curr, const char *start)
{
    unsigned int line, column, count;
    const char *walk;
    int size;

    column = 1;
    line = 1;

    for (walk = start; walk <= curr; ++walk) {
        if (*walk != '\n') {
            column++;
            continue;
        }

        start = walk;
        column = 0;
        line++;
    }

    walk = strchr(walk, '\n');
    size = walk ? walk - ++start: -1;

    bfdev_log_err("Line Number %u, Column %u\n", line, column);
    printf("\t| %.*s\n", size, start);

    printf("\t| ");
    for (count = 1; count < column; ++count)
        printf("-");
    printf("^\n");
}

int main(int argc, const char *argv[])
{
    const char *errinfo, *end;
    int index, retval, fd;
    struct stat stat;
    void *block;

    for (index = 1; index < argc; ++index) {
        if ((fd = open(argv[index], O_RDONLY)) < 0)
            err(1, argv[index]);

        if ((retval = fstat(fd, &stat)) < 0)
            err(retval, argv[index]);

        block = mmap(NULL, stat.st_size, PROT_READ, MAP_SHARED, fd, 0);
        if (block == MAP_FAILED)
            err(1, argv[index]);

        retval = brainfuck(block, &end);
        if (bfdev_likely(!retval))
            continue;

        bfdev_errname(retval, &errinfo);
        bfdev_log_err("execute error: %s\n", errinfo);
        show_error(end, block);

        munmap(block, stat.st_size);
        close(fd);

        return retval;
    }

    return 0;
}
