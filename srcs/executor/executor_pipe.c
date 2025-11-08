/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pipe.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 16:05:37 by maborges          #+#    #+#             */
/*   Updated: 2025/11/06 16:28:41 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

//should I take care of dup2 fail?

static void	left_pipe(t_pipeline *pipeline, t_mshell *shell, int *fds)
{
	default_child_signals();
	close(fds[0]);
	dup2(fds[1], STDOUT_FILENO);
	close(fds[1]);
	execute_ast(pipeline->left, shell);
	exit(shell->exit_status);
}

static void	right_pipe(t_pipeline *pipeline, t_mshell *shell, int *fds)
{
	default_child_signals();
	close(fds[1]);
	dup2(fds[0], STDIN_FILENO);
	close(fds[0]);
	execute_ast(pipeline->right, shell);
	exit(shell->exit_status);
}

static void	pipe_signal_handler(pid_t left_pid, pid_t right_pid)
{
	kill(left_pid, SIGINT);
	kill(right_pid, SIGINT);
	g_signal_received = 0;
}

static void	status_handler(int status, t_mshell *shell)
{
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		shell->exit_status = 128 + WTERMSIG(status);
}

void	execute_pipe(t_ast *pipe_node, t_mshell *shell)
{
	int			fds[2];
	// fd[0] - read
	// fd[1] - write
	pid_t		left_pid;
	pid_t		right_pid;
	int			status;
	t_pipeline	*pipeline;

	pipeline = (t_pipeline *)pipe_node;
	if (pipe(fds) == -1)
		return (perror("minishell: pipe"));
	setup_non_interactive_signals();
	left_pid = fork();
	if (left_pid == 0)
		left_pipe(pipeline, shell, fds);
	right_pid = fork();
	if (right_pid == 0)
		right_pipe(pipeline, shell, fds);
	close(fds[0]);
	close(fds[1]);
	setup_interactive_signals();
	if (g_signal_received == SIGINT)
		pipe_signal_handler(left_pid, right_pid);
	waitpid(left_pid, &status, 0);
	waitpid(right_pid, &status, 0);
	status_handler(status, shell); // Should I take care of status separately?
	setup_interactive_signals();
}
