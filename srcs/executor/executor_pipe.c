/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_pipe.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 16:05:37 by maborges          #+#    #+#             */
/*   Updated: 2025/10/07 16:06:56 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	execute_pipe(t_ast *pipe_node, t_mshell *shell)
{
    int		pipe_fds[2];
    pid_t	left_pid;
    pid_t	right_pid;
    int		status;

    if (pipe(pipe_fds) == -1)
    {
        perror("minishell: pipe");
        return ;
    }

    // Fork for left side
    left_pid = fork();
    if (left_pid == 0)
    {
        // Child process: restore default signal behavior
        restore_default_signals();
        
        close(pipe_fds[0]);  // Close read end
        dup2(pipe_fds[1], STDOUT_FILENO);  // Redirect stdout to pipe
        close(pipe_fds[1]);

        // RECURSIVE execution of left side!
        execute_ast(pipe_node->left, shell);
        exit(shell->exit_status);
    }

    // Fork for right side
    right_pid = fork();
    if (right_pid == 0)
    {
        // Child process: restore default signal behavior
        restore_default_signals();
        
        close(pipe_fds[1]);  // Close write end
        dup2(pipe_fds[0], STDIN_FILENO);  // Redirect stdin from pipe
        close(pipe_fds[0]);

        // RECURSIVE execution of right side!
        execute_ast(pipe_node->right, shell);
        exit(shell->exit_status);
    }

    // Parent closes both ends and waits
    close(pipe_fds[0]);
    close(pipe_fds[1]);
    
    // Restore interactive signals in parent
    setup_interactive_signals();
    
    // Check for SIGINT during pipe execution
    if (g_signal_received == SIGINT)
    {
        kill(left_pid, SIGINT);
        kill(right_pid, SIGINT);
        g_signal_received = 0;
    }
    
    waitpid(left_pid, &status, 0);
    waitpid(right_pid, &status, 0);

    if (WIFEXITED(status))
        shell->exit_status = WEXITSTATUS(status);
    else if (WIFSIGNALED(status))
        shell->exit_status = 128 + WTERMSIG(status);
}
