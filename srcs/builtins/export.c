#include "../../inc/parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

extern char **environ;

static int is_valid_identifier(const char *s)
{
    if (!s || !*s) return 0;
    if (!(isalpha((unsigned char)*s) || *s == '_')) return 0;
    s++;
    while (*s && *s != '=')
    {
        if (!(isalnum((unsigned char)*s) || *s == '_')) return 0;
        s++;
    }
    return 1;
}

int builtin_export(char **args)
{
    // No args: print environment (simplified)
    if (!args[1])
    {
        for (char **e = environ; e && *e; ++e)
            printf("declare -x %s\n", *e);
        return 0;
    }

    int status = 0;
    for (int i = 1; args[i]; ++i)
    {
        if (!is_valid_identifier(args[i]))
        {
            fprintf(stderr, "export: `%s': not a valid identifier\n", args[i]);
            status = 1;
            continue;
        }

        char *eq = strchr(args[i], '=');
        if (!eq)
        {
            // In bash, 'export KEY' marks it for export; for simplicity, ensure var exists
            const char *val = getenv(args[i]);
            if (!val) setenv(args[i], "", 0);
            continue;
        }

        *eq = '\0';
        const char *key = args[i];
        const char *value = eq + 1;
        if (setenv(key, value, 1) != 0)
        {
            perror("export");
            status = 1;
        }
        *eq = '='; // restore
    }
    return status;
}
