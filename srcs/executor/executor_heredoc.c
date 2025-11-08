/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_heredoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 18:47:22 by maborges          #+#    #+#             */
/*   Updated: 2025/11/08 18:54:47 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// Heredoc signal handler - aborts heredoc on SIGINT
// heredoc redirects input from a temporary file that has lines of text, used with ( << ) oeprator
void	heredoc_signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		g_signal_received = SIGINT;
		// Exit heredoc process
		exit(130);
	}
}

// Setup signals for heredoc input
void	setup_heredoc_signals(void)
{
	struct sigaction sa_int;
	struct sigaction sa_quit;

	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	sa_int.sa_handler = heredoc_signal_handler;
	sigaction(SIGINT, &sa_int, NULL);

	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sa_quit.sa_handler = SIG_IGN; // Ignore SIGQUIT in heredoc
	sigaction(SIGQUIT, &sa_quit, NULL);
}

// Create heredoc file with proper signal handling
// heredoc redirects input from a temp file that has lines of text, used with ( << ) oeprator
// shell reads until it finds the delimiter, uses content as input for command

int	create_heredoc_file(char *delimiter)
{
	int		fd;
	char	*line;
	char	*tmp_file;
	pid_t	pid;
	int		status;

	// Create temporary file
	tmp_file = "/tmp/minishell_heredoc";
	fd = open(tmp_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
		return (-1);
	close(fd);

	// Fork to handle heredoc input
	pid = fork();
	if (pid == 0)
	{
		// Child: setup heredoc signals and read input
		setup_heredoc_signals();
		fd = open(tmp_file, O_WRONLY);
		if (fd == -1)
			exit(1);

		while (1)
		{
			line = readline("> ");
			if (!line) // Ctrl+D closes heredoc
			{
				close(fd);
				exit(0);
			}
			if (ft_strcmp(line, delimiter) == 0)
			{
				free(line);
				close(fd);
				exit(0);
			}
			ft_putstr_fd(line, fd);
			ft_putstr_fd("\n", fd);
			free(line);
		}
	}
	else
	{
		// Parent: wait for child and handle signals
		waitpid(pid, &status, 0);
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		{
			unlink(tmp_file);
			return (-1);
		}
	}
	// Reopen file for reading
	fd = open(tmp_file, O_RDONLY);
	unlink("/tmp/heredoc_tmp");
	return (fd);
}
