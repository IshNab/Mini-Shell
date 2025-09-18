#include "../../inc/parser.h"
#include "../../libraries/libft.h"
#include "../../libraries/ft_printf.h"
#include <stdio.h>
#include <stdlib.h>

extern char **environ;
/*
static int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}*/

static void	print_env_vars(void)
{
	int	i;

	i = 0;
	while (environ[i])
	{
		ft_printf("%s\n", environ[i]);
		i++;
	}
}

static int	handle_env_with_args(char **args)
{
	int	i;
	int	command_start;

	i = 1;
	command_start = 1;
	while (args[i])
	{
		if (ft_strchr(args[i], '='))
		{
			// Set environment variable temporarily
			if (putenv(args[i]) != 0)
			{
				perror("env");
				return (1);
			}
			command_start = i + 1;
		}
		else
		{
			// Found command to execute
			break;
		}
		i++;
	}
	if (args[command_start])
	{
		// Execute command with modified environment
		// For now, just print the environment and return
		// In a full implementation, you'd execute the command here
		ft_printf("env: would execute: %s\n", args[command_start]);
	}
	else
	{
		// No command, just print environment
		print_env_vars();
	}
	return (0);
}

int	builtin_env(char **args)
{
	if (!args[1])
	{
		// No arguments - print all environment variables
		print_env_vars();
		return (0);
	}
	else
	{
		// Handle arguments (variable assignments and/or commands)
		return (handle_env_with_args(args));
	}
}
