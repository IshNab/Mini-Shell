/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 17:53:39 by maborges          #+#    #+#             */
/*   Updated: 2025/09/27 21:33:07 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	run_external_cmd(t_command *cmd, t_mshell *shell)
{
	char	*path;
	char	**env_array;

	path = find_cmd_path(cmd->args[0], &shell->env);
	if (!path)
	{
		perror("command not found"); //make a good error_msg function that handle fd and variables
		shell->exit_status = 127;
		exit(127);
	}
	env_array = env_to_array(shell->env); //TODO
	if (execve(path, cmd->args, env_array) == -1)
	{
		perror("execve failed");
		free(path);
		shell->exit_status = 127;
		exit(126);
	}
	free(path);
}

static int	try_builtin(t_command *cmd, t_mshell *shell)
{
	if (!cmd->args[0])
		return (0);
	if (!ft_strcmp(cmd->args[0], "cd"))
	{
		shell->exit_status = builtin_cd(cmd->args);
		return (1);
	}
	if (!ft_strcmp(cmd->args[0], "pwd"))
	{
		shell->exit_status = builtin_pwd(cmd->args);
		return (1);
	}
	/*if(strcmp(cmd->args[0], "echo") == 0) return builtin_echo(cmd->args);
	if (strcmp(cmd->args[0], "env") == 0) return builtin_env(cmd->args);
	if (strcmp(cmd->args[0], "exit") == 0) return builtin_exit(cmd->args);
	if (strcmp(cmd->args[0], "export") == 0) return builtin_export(cmd->args); */
	//if (strcmp(cmd->args[0], "unset") == 0) return builtin_unset(cmd->args);
	else
		return (0);
}

void	execute_command(t_command *cmd, t_mshell *shell)
{
	int	status;
	pid_t	child_pid;

	if (try_builtin(cmd, shell))
		return ;
	child_pid = fork_wrapper();
	if (child_pid == 0)
	{
		run_external_cmd(cmd, shell); //TODO͘
		exit(127);
	}
	waitpid(child_pid, &status, 0);
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	else
		shell->exit_status = 128 + WTERMSIG(status);
	//should I take also care of the return is -1?
	return ;
}

/* int	execute_ast(t_ast *node, t_mshell *mshell)
{
	(void)mshell;
	if (node->type == NODE_CMD)
	{
		if (try_builtin((t_command *)node))
			return (0);
		else
			execute_command((t_command *)node);
	}
	//if (node->type == NODE_PIPE)
	//{
	//	execute_pipe((t_pipeline *)node);
		//execute and next step
	//}
	return (1);
} */
