#include "../../inc/parser.h"
#include "../../libraries/libft.h"
#include <stdio.h>

int builtin_unset(char **args)
{
    if (!args[1])
    {
        ft_printf("unset: missing argument\n");
        return 1;
    }
    if (unsetenv(args[1]) != 0)
    {
        perror("unset");
        return 1;
    }
    return 0;
}
