#ifndef PARSER_H
#define PARSER_H

int parse_command(char *input);
int execute_command(char **args);

int builtin_cd(char **args);
int builtin_echo(char **args);
int builtin_env(char **args);
int builtin_exit(char **args);
int builtin_export(char **args);
int builtin_pwd(char **args);
int builtin_unset(char **args);

#endif
