#include "../../inc/parser.h"
#include "../../libraries/libft.h"
#include "../../libraries/ft_printf.h"
#include <stdlib.h>
#include <stdio.h>

int builtin_exit(char **args) 
{
    int status = 0;
    if (args[1])
        status = atoi(args[1]);
    ft_printf("exit\n");
    exit(status);
    return 0;
}
