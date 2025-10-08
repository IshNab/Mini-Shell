#include "../inc/minishell.h"

static int is_valid_identifier(const char *s)
{
    if (!s || !*s) return 0;
    if (!(ft_isalpha((unsigned char)*s) || *s == '_')) return 0;
    s++;
    while (*s)
    {
        if (!(ft_isalnum((unsigned char)*s) || *s == '_')) return 0;
        s++;
    }
    return 1;
}

int builtin_unset(char **args, t_mshell *shell)
{
	(void)shell;
    int status = 0;
    for (int i = 1; args[i]; ++i)
    {
        if (!is_valid_identifier(args[i]))
        {
            fprintf(stderr, "unset: `%s': not a valid identifier\n", args[i]);
            status = 1;
            continue;
        }
        if (unsetenv(args[i]) != 0)
        {
            perror("unset");
            status = 1;
        }
    }
    return status;
}
