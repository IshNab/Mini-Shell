#include "../../inc/parser.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <ctype.h>

static int is_numeric(const char *s)
{
    if (!s || !*s) return 0;
    if (*s == '+' || *s == '-') s++;
    while (*s)
    {
        if (!isdigit((unsigned char)*s)) return 0;
        s++;
    }
    return 1;
}

int builtin_exit(char **args)
{
    int argc = 0;
    while (args[argc]) argc++;

    fprintf(stderr, "exit\n");

    if (argc == 1)
        exit(0);

    if (!is_numeric(args[1]))
    {
        fprintf(stderr, "exit: %s: numeric argument required\n", args[1]);
        exit(2);
    }

    if (argc > 2)
    {
        fprintf(stderr, "exit: too many arguments\n");
        return 1; // do not exit, as bash does
    }

    // Convert to unsigned char semantics (0-255)
    long long val = strtoll(args[1], NULL, 10);
    exit((unsigned char)val);
}
