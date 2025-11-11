/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 17:53:39 by maborges          #+#    #+#             */
/*   Updated: 2025/11/11 15:52:57 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	has_redirections(t_command *cmd)
{
	if (cmd->input_file || cmd->output_file || cmd->heredoc_delimiter)
		return (1);
	return (0);
}

static int	execute_builtin_parent(t_command *cmd, t_mshell *shell)
{
	int	saved_stdin;
	int	saved_stdout;

	if (has_redirections(cmd))
	{
		if (setup_redirections(cmd, &saved_stdin, &saved_stdout) == -1)
			return (shell->exit_status = 1, 1);
	}
	else
	{
		saved_stdin = -1;
		saved_stdout = -1;
	}
	try_builtin(cmd, shell);
	if (has_redirections(cmd))
		restore_redirections(saved_stdin, saved_stdout);
	return (1);
}

static void	exec_in_child(t_command *cmd, t_mshell *shell)
{
	int	saved_stdin;
	int	saved_stdout;
	default_child_signals();
	if (has_redirections(cmd))
	{
		if (setup_redirections(cmd, &saved_stdin, &saved_stdout) == -1)
			exit(1);
	}
	if (try_builtin(cmd, shell)) //(echo, pwd, env can run in child)?
		exit(shell->exit_status);
	run_external_cmd(cmd, shell);
	exit(127); //if reaches here something went wrong, ouput error 127
}

int	execute_simple_command(t_ast *ast, t_mshell *shell)
{
	t_command	*cmd;
	int			status;
	pid_t		child_pid;

	if (!ast)
		return (1);
	cmd = (t_command *)ast;
	if (!cmd->args || !cmd->args[0])
		return (1);
	if (is_parent_builtin(cmd->args[0])) //Can be run only in parent
		return (execute_builtin_parent(cmd, shell));
	setup_non_interactive_signals();
	child_pid = fork_wrapper(shell);
	if (child_pid == -1)
		return (1);
	if (child_pid == 0)
		exec_in_child(cmd, shell);
	if (g_signal_received == SIGINT) //Check if found SIGINT
	{
		kill(child_pid, SIGINT);
		g_signal_received = 0;
	}
	waitpid(child_pid, &status, 0);
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		shell->exit_status = 128 + WTERMSIG(status);
	return (0);
}

void	execute_ast(t_ast *ast, t_mshell *shell)
{
	//this should be recursive!??
	if (ast->type == NODE_PIPE)
	{
		execute_pipe(ast, shell);
	}
	else if (ast->type == NODE_CMD)
	{
		if (execute_simple_command(ast, shell) == -1)
			return ;
	}
}
