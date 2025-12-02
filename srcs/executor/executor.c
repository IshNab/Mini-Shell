/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 17:53:39 by maborges          #+#    #+#             */
/*   Updated: 2025/12/02 15:48:43 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
//(echo, pwd, env run in child

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
	if (try_builtin(cmd, shell))
		exit(shell->exit_status);
	run_external_cmd(cmd, shell);
	exit(shell->exit_status);
}

int	execute_simple_command(t_ast *ast, t_mshell *shell)
{
	t_command	*cmd;
	pid_t		child_pid;

	cmd = (t_command *)ast;
	if (!cmd->args || !cmd->args[0])
		return (1);
	if (is_parent_builtin(cmd->args[0]))
		return (execute_builtin_parent(cmd, shell));
	setup_non_interactive_signals();
	child_pid = fork_wrapper(shell);
	if (child_pid == -1)
		return (1);
	if (child_pid == 0)
		exec_in_child(cmd, shell);
	wait_child_process(child_pid, shell);
	return (0);
}

void	execute_ast(t_ast *ast, t_mshell *shell)
{
	if (!ast)
		return ;
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
