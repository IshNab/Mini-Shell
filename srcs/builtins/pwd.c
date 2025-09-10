#include "../parser.h"
#include <stdio.h>
#include <unistd.h>

int pwd(char **args)
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
        printf("%s\n", cwd);
    else
        perror("pwd");
    return 0;
}
