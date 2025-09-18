#include "../../inc/parser.h"
#include "../../libraries/libft.h"
#include "../../libraries/ft_printf.h"
#include <sys/wait.h>
#include <unistd.h>

int execute_command(char **args);
int has_redirection(char *input);
int has_pipe(char *input);
char *ft_strtok_custom(char *str, const char *delim);

int parse_command(char *input)
{
    char *args[100];
    int argc;
    char *token;
    int i;

    if (has_pipe(input))
    {
        ft_printf("Pipe detected: %s\n", input);
        return (0);
    }
    if (has_redirection(input))
    {
        ft_printf("Redirection detected: %s\n", input);
        return (0);
    }
    argc = 0;
    token = ft_strtok_custom(input, " \t\n");
    while (token && argc < 99)
    {
        args[argc++] = token;
        token = ft_strtok_custom(NULL, " \t\n");
    }
    args[argc] = NULL;
    if (argc == 0)
        return (0);
    if (ft_strncmp(args[0], "cd", 3) == 0)
        return (builtin_cd(args));
    if (ft_strncmp(args[0], "echo", 5) == 0)
        return (builtin_echo(args));
    if (ft_strncmp(args[0], "env", 4) == 0)
        return (builtin_env(args));
    if (ft_strncmp(args[0], "exit", 5) == 0)
        return (builtin_exit(args));
    if (ft_strncmp(args[0], "export", 7) == 0)
        return (builtin_export(args));
    if (ft_strncmp(args[0], "pwd", 4) == 0)
        return (builtin_pwd(args));
    if (ft_strncmp(args[0], "unset", 6) == 0)
        return (builtin_unset(args));
    return (execute_command(args));
}

int execute_command(char **args) {
    pid_t pid;
    int status;

    pid = fork();
    if (pid == 0) {
        // Child process
        if (execvp(args[0], args) == -1) {
            perror("minishell");
            exit(1);
        }
    } else if (pid < 0) {
        perror("fork");
        return 1;
    } else {
        // Parent process
        waitpid(pid, &status, 0);
        return WEXITSTATUS(status);
    }
    return 0;
}

// Check if input contains redirection operators
int has_redirection(char *input)
{
    return (ft_strnstr(input, "<", ft_strlen(input)) != NULL || 
            ft_strnstr(input, ">", ft_strlen(input)) != NULL || 
            ft_strnstr(input, ">>", ft_strlen(input)) != NULL || 
            ft_strnstr(input, "<<", ft_strlen(input)) != NULL);
}

// Check if input contains pipe operator
int has_pipe(char *input)
{
    return (ft_strnstr(input, "|", ft_strlen(input)) != NULL);
}


// Custom tokenizer function
char *ft_strtok_custom(char *str, const char *delim)
{
    static char *last;
    char *start;
    char *end;

    if (str != NULL)
        last = str;
    if (last == NULL || *last == '\0')
        return (NULL);
    start = last;
    while (*last && ft_strchr(delim, *last) == NULL)
        last++;
    if (*last == '\0')
    {
        last = NULL;
        return (start);
    }
    *last = '\0';
    last++;
    return (start);
}
