#include "../parser.h"
#include <stdio.h>
#include <unistd.h>

int cd(char **args)
{
    if (!args[1]) {
        fprintf(stderr, "cd: missing argument\n");
        return 1;
    }
    if (chdir(args[1]) != 0) {
        perror("cd");
        return 1;
    }
    return 0;
}
