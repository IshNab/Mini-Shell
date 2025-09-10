#include "../parser.h"
#include <stdio.h>

extern char **environ;

int env(char **args)
{
    int i = 0;
    while (environ[i]) {
        printf("%s\n", environ[i]);
        i++;
    }
    return 0;
}
