/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 17:53:39 by maborges          #+#    #+#             */
/*   Updated: 2025/09/22 19:31:18 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	run_builtin(t_cmd *cmd)
{
	if(strcmp(cmd->args[0], "cd") == 0)
		return builtin_cd(cmd->args); //strcmp not allowed
	if(strcmp(cmd->args[0], "echo") == 0) return builtin_echo(cmd->args);
	if (strcmp(cmd->args[0], "env") == 0) return builtin_env(cmd->args);
	if (strcmp(cmd->args[0], "exit") == 0) return builtin_exit(cmd->args);
	if (strcmp(cmd->args[0], "export") == 0) return builtin_export(cmd->args);
	if (strcmp(cmd->args[0], "pwd") == 0) return builtin_pwd(cmd->args);
	if (strcmp(cmd->args[0], "unset") == 0) return builtin_unset(cmd->args);
}

static int	is_builtin(char *cmd)
{
	if (cmd == "cd" || cmd == "echo" || cmd == "env" || cmd == "exit"
		|| cmd == "export" || cmd == "pwd" || cmd == "unset")
		return (1);
	else
		return (0);
}

int	exec_cmd(t_cmd *cmd, char **envp)
{
	if (!is_builtin(cmd->args[0]))
		return run_builtin(cmd); //TODO
	else
		fork_wrapper(); //TODO
	return (0);
}
