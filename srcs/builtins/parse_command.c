#include "../../inc/parser.h"
#include <string.h>

int parse_command(char *input) {
    // Simple parser: split input and call built-in if matched
    char *args[100];
    int argc = 0;
    char *token = strtok(input, " \t\n");
    while (token && argc < 99) {
        args[argc++] = token;
        token = strtok(NULL, " \t\n");
    }
    args[argc] = NULL;

    if (argc == 0) return 0;

    if (strcmp(args[0], "cd") == 0) return builtin_cd(args);
    if (strcmp(args[0], "echo") == 0) return builtin_echo(args);
    if (strcmp(args[0], "env") == 0) return builtin_env(args);
    if (strcmp(args[0], "exit") == 0) return builtin_exit(args);
    if (strcmp(args[0], "export") == 0) return builtin_export(args);
    if (strcmp(args[0], "pwd") == 0) return builtin_pwd(args);
    if (strcmp(args[0], "unset") == 0) return builtin_unset(args);

    // Not a built-in
    return -1;
}
