#include "../parser.h"
#include <stdio.h>
#include <stdlib.h>

int export(char **args)
{
    if (!args[1]) {
        fprintf(stderr, "export: missing argument\n");
        return 1;
    }
    char *key = args[1];
    char *value = strchr(key, '=');
    if (!value) {
        fprintf(stderr, "export: invalid format, use KEY=VALUE\n");
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
