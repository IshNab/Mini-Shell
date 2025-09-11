#include "../../inc/parser.h"
#include <stdio.h>
#include <unistd.h>

int builtin_pwd(char **args)
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
        printf("%s\n", cwd);
    else
        perror("pwd");
    return 0;
}
