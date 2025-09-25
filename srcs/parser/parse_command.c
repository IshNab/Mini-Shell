/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inabakka <inabakka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 14:35:26 by maborges          #+#    #+#             */
/*   Updated: 2025/09/25 13:46:10 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static void	init_t_cmd(t_cmd *cmd, char **args)
{
	int	i;

	i = 0;
	while (args[i])
	{
		cmd->args = &args[i];
		i++;
	}
	cmd->input_file = NULL;
	cmd->output_file = NULL;
	cmd->append = 0;
}

int	parse_command(char *input, char **envp)
{
	char	*args[100];
	int		argc;
	t_cmd	cmd;
	char	*token;

	argc = 0;
	token = ft_strtok(input, " \t\n");
	while (token && argc < 99)
	{
		args[argc] = token;
		argc++;
		token = ft_strtok(NULL, " \t\n");
	}
	args[argc] = NULL;
	if (argc == 0)
		return (1);
	init_t_cmd(&cmd, args);
	exec_cmd(&cmd, envp);
	return (0);
}
