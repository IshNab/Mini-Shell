#include "../../inc/parser.h"
#include "../../libraries/libft.h"
#include <stdio.h>
#include <stdlib.h>

int builtin_export(char **args)
{
    extern char **environ;
    int i = 0;
    
    if (!args[1]) {
        // Display all exported variables
        while (environ[i]) {
            ft_printf("declare -x %s\n", environ[i]);
            i++;
        }
        return 0;
    }
    
    char *key = args[1];
    char *value = ft_strchr(key, '=');
    if (!value) {
        ft_printf("export: invalid format, use KEY=VALUE\n");
        return 1;
    }
    *value = '\0';
    value++;
    if (setenv(key, value, 1) != 0) {
        perror("export");
        return 1;
    }
    return 0;
}
