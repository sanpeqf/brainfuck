/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <stdio.h>
#include <bfdev.h>

static inline int
execute(const char *walk)
{
    BFDEV_DEFINE_ARRAY(array, NULL, BFDEV_BYTES_PER_CHAR);
    unsigned int deepth, index = -1;
    const char *btrack = NULL;
    char *reg, ch = '>';

    do switch (ch) {
        case '>':
            if (++index < bfdev_array_index(&array))
                goto getreg;
            reg = bfdev_array_push(&array, 1);
            if (bfdev_unlikely(!reg))
                return -BFDEV_ENOMEM;
            *reg = 0;
            break;

        case '<':
            index--;
            goto getreg;

        getreg:
            reg = bfdev_array_data(&array, index);
            if (bfdev_unlikely(!reg))
                return -BFDEV_EOVERFLOW;
            break;

        case '+':
            (*reg)++;
            break;

        case '-':
            (*reg)--;
            break;

        case '.':
            putchar(*reg);
            break;

        case ',':
            *reg = getchar();
            break;

        case '[':
            btrack = walk;
            if (*reg)
                break;
            deepth = 1;
            while (deepth) {
                if (!*walk)
                    return -BFDEV_EINVAL;
                if (*walk == '[')
                    deepth++;
                else if (*walk == ']')
                    deepth--;
                walk++;
            }
            break;

        case ']':
            if (!*reg)
                break;
            walk = btrack;
            if (bfdev_unlikely(!walk))
                return -BFDEV_ENOKEY;
            break;

        default:
            break;
    } while ((ch = *walk++));

    return 0;
}

int main(int argc, const char *argv[])
{
    const char *errinfo;
    int index, retval;

    for (index = 1; index < argc; ++index) {
        retval = execute(argv[index]);
        if (bfdev_unlikely(retval)) {
            bfdev_errname(retval, &errinfo);
            printf("execute error: %s\n", errinfo);
            return retval;
        }
    }

    return 0;
}
