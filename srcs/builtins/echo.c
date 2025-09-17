#include "../../inc/parser.h"
#include "../../libraries/libft.h"
#include <stdio.h>

int builtin_echo(char **args)
{
    int i = 1;
    int newline = 1;
    
    // Check for -n option
    if (args[1] && ft_strcmp(args[1], "-n") == 0) {
        newline = 0;
        i = 2;
    }
    
    while (args[i]) {
        ft_printf("%s", args[i]);
        if (args[i + 1])
            ft_printf(" ");
        i++;
    }
    
    if (newline)
        ft_printf("\n");
    return 0;
}
