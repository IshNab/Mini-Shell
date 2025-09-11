#include "../../inc/parser.h"
#include <stdio.h>

extern char **environ;

int builtin_env(char **args)
{
    (void)args; // env ignores extra args in many shells; 42 subject might require error if args exist
    for (char **e = environ; e && *e; ++e)
        puts(*e);
    return 0;
}
