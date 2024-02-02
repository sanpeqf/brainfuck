/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright(c) 2023 John Sanpe <sanpeqf@gmail.com>
 */

#include <stdio.h>
#include <bfdev.h>

int
brainfuck(const char *syntax, const char **endp)
{
    BFDEV_DEFINE_ARRAY(array, NULL, BFDEV_BYTES_PER_CHAR);
    BFDEV_DEFINE_ARRAY(stack, NULL, BFDEV_BYTES_PER_UINTPTR);
    unsigned int deepth, index;
    const char **btrackp;
    char *reg, ch;
    int retval;

    retval = 0;
    index = -1;
    ch = '>';

    do switch (ch) {
        case '>':
            if (++index < bfdev_array_index(&array))
                goto getreg;

            reg = bfdev_array_push(&array, 1);
            if (bfdev_unlikely(!reg)) {
                retval = -BFDEV_ENOMEM;
                goto finish;
            }

            *reg = 0;
            break;

        case '<':
            index--;
            goto getreg;

        getreg:
            reg = bfdev_array_data(&array, index);
            if (bfdev_unlikely(!reg)) {
                retval = -BFDEV_EOVERFLOW;
                goto finish;
            }
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
            if (!btrackp) {
                retval = -BFDEV_ENOMEM;
                goto finish;
            }

            *btrackp = syntax;
            if (*reg)
                break;

            for (deepth = 1; deepth; ++syntax) {
                if (!*syntax) {
                    retval = -BFDEV_EINVAL;
                    goto finish;
                }

                if (*syntax == '[')
                    deepth++;
                else if (*syntax == ']')
                    deepth--;

                syntax++;
            }
            break;

        case ']':
            btrackp = bfdev_array_peek(&stack, 1);
            if (bfdev_unlikely(!btrackp)) {
                retval = -BFDEV_ENOKEY;
                goto finish;
            }

            if (!*reg) {
                bfdev_array_pop(&stack, 1);
                break;
            }

            syntax = *btrackp;
            break;

        default:
            break;
    } while ((ch = *syntax++));

finish:
    bfdev_array_release(&array);
    bfdev_array_release(&stack);

    *endp = syntax - 1;
    return retval;
}
