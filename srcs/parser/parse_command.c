/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 14:35:26 by maborges          #+#    #+#             */
/*   Updated: 2025/09/19 18:30:43 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// Simple parser: split input and call built-in if matched
int	parse_command(char *input)
{
	char	*args[100];
	int		argc = 0;
	char	*token = strtok(input, " \t\n"); // strtok not allowed
	while (token && argc < 99)
	{
		args[argc++] = token;
		token = strtok(NULL, " \t\n");
	}
	args[argc] = NULL;
	if (argc == 0) return 0;
	if(strcmp(args[0], "cd") == 0) return builtin_cd(args); //strcmp not allowed
	if(strcmp(args[0], "echo") == 0) return builtin_echo(args);
	if (strcmp(args[0], "env") == 0) return builtin_env(args);
	if (strcmp(args[0], "exit") == 0) return builtin_exit(args);
	if (strcmp(args[0], "export") == 0) return builtin_export(args);
	if (strcmp(args[0], "pwd") == 0) return builtin_pwd(args);
	if (strcmp(args[0], "unset") == 0) return builtin_unset(args);
	// Not a built-in
	return -1;
}
