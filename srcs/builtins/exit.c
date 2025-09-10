#include "../parser.h"
#include <stdlib.h>
#include <stdio.h>

int exit(char **args) 
{
    int status = 0;
    if (args[1])
        status = atoi(args[1]);
    printf("exit\n");
    exit(status);
    printf("exit called\n");
    return 0;
}
