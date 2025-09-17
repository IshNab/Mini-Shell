#include "../../inc/parser.h"
#include "../../libraries/libft.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int builtin_cd(char **args)
{
    char *path;
    
    if (!args[1] || ft_strcmp(args[1], "~") == 0) {
        path = getenv("HOME");
        if (!path) {
            ft_printf("cd: HOME not set\n");
            return 1;
        }
    } else if (ft_strcmp(args[1], "-") == 0) {
        path = getenv("OLDPWD");
        if (!path) {
            ft_printf("cd: OLDPWD not set\n");
            return 1;
        }
    } else {
        path = args[1];
    }
    
    if (chdir(path) != 0) {
        perror("cd");
        return 1;
    }
    return 0;
}
