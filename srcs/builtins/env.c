#include "../../inc/parser.h"
#include "../../libraries/libft.h"
#include <stdio.h>

extern char **environ;

int builtin_env(char **args)
{
    int i = 0;
    while (environ[i]) {
        ft_printf("%s\n", environ[i]);
        i++;
    }
    return 0;
}
