/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 17:53:39 by maborges          #+#    #+#             */
/*   Updated: 2025/09/24 17:55:55 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"


void	execute_command(t_command * node)
{
	if (fork_wrapper() == 0) //TODO
		return (1);//runcmd(); //TODO
	wait(0);
}

static int	try_builtin(t_command *node)
{
	if (!ft_strcmp(node->args[0], "cd"))
		return (builtin_cd(node->args));
	/*if(strcmp(cmd->args[0], "echo") == 0) return builtin_echo(cmd->args);
	if (strcmp(cmd->args[0], "env") == 0) return builtin_env(cmd->args);
	if (strcmp(cmd->args[0], "exit") == 0) return builtin_exit(cmd->args);
	if (strcmp(cmd->args[0], "export") == 0) return builtin_export(cmd->args); */
	if (!strcmp(node->args[0], "pwd")) return builtin_pwd(node->args);
	//if (strcmp(cmd->args[0], "unset") == 0) return builtin_unset(cmd->args);
	else
		return (0);
}

int	executor_cmd_type(t_ast *node)
{
	if (node->type == NODE_CMD)
	{
		if (try_builtin((t_command *)node))
			return (0);
		else
			execute_command((t_command *)node);
	}
	/*if (node->type == NODE_PIPE)
	{
		execute_pipe((t_pipeline *)node);
		//execute and next step
	} */
	return (1);
}
