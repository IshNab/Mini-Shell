/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 14:35:26 by maborges          #+#    #+#             */
/*   Updated: 2025/09/22 19:18:54 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static void	init_t_cmd(t_cmd *cmd)
{
	cmd->args = (char *[]){"ls", "-la"};
	cmd->input_file = NULL;
	cmd->output_file = NULL;
	cmd->append = 0;
}

// Simple parser: split input and call built-in if matched
int	parse_command(char *input, char **envp)
{
	char	*args[100];
	int		argc = 0;
	t_cmd	cmd;
	char	*token = strtok(input, " \t\n"); // strtok not allowed
	while (token && argc < 99)
	{
		args[argc++] = token;
		token = strtok(NULL, " \t\n");
	}
	args[argc] = NULL;
	if (argc == 0)
		return (1);
	init_t_cmd(&cmd);
	exec_cmd(&cmd, envp);
	return (0);
}
