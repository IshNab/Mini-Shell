#include "../parser.h"
#include <stdio.h>

int echo(char **args)
{
    int i = 1;
    while (args[i]) {
        printf("%s", args[i]);
        if (args[i + 1])
            printf(" ");
        i++;
    }
    printf("\n");
    return 0;
}
