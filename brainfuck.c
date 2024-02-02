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
    BFDEV_DEFINE_ARRAY(stack, NULL, BFDEV_BYTES_PER_UINTPTR);
    unsigned int deepth, index;
    const char **btrackp;
    char *reg, ch;

    index = -1;
    ch = '>';

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
            btrackp = bfdev_array_push(&stack, 1);
            if (!btrackp)
                return -BFDEV_ENOMEM;

            *btrackp = walk;
            if (*reg)
                break;

            for (deepth = 1; deepth; ++walk) {
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
            if (!*reg) {
                bfdev_array_pop(&stack, 1);
                break;
            }

            btrackp = bfdev_array_peek(&stack, 1);
            if (bfdev_unlikely(!btrackp))
                return -BFDEV_ENOKEY;

            walk = *btrackp;
            break;

        default:
            break;
    } while ((ch = *walk++));

    bfdev_array_release(&array);
    bfdev_array_release(&stack);

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
