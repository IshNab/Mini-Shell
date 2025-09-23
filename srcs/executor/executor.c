/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 17:53:39 by maborges          #+#    #+#             */
/*   Updated: 2025/09/23 16:17:37 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"


static int	try_builtin(t_cmd *cmd)
{
	if (!ft_strcmp(cmd->args[0], "cd"))
		return (builtin_cd(cmd->args));
	/*if(strcmp(cmd->args[0], "echo") == 0) return builtin_echo(cmd->args);
	if (strcmp(cmd->args[0], "env") == 0) return builtin_env(cmd->args);
	if (strcmp(cmd->args[0], "exit") == 0) return builtin_exit(cmd->args);
	if (strcmp(cmd->args[0], "export") == 0) return builtin_export(cmd->args); */
	if (!strcmp(cmd->args[0], "pwd")) return builtin_pwd(cmd->args);
	//if (strcmp(cmd->args[0], "unset") == 0) return builtin_unset(cmd->args);
	else
		return (0);
}

int	exec_cmd(t_cmd *cmd, char **envp)
{
	(void)envp;
	if (try_builtin(cmd))
		return (0);
	if (fork_wrapper() == 0) //TODO
		return (1);//runcmd(); //TODO
	wait (0);
	return (1);
}
