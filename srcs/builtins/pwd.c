#include "../../inc/parser.h"
#include "../../libraries/libft.h"
#include <stdio.h>
#include <unistd.h>

int builtin_pwd(char **args)
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL)
        ft_printf("%s\n", cwd);
    else
        perror("pwd");
    return 0;
}
