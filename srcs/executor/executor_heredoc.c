/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_heredoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 18:47:22 by maborges          #+#    #+#             */
/*   Updated: 2025/11/12 16:10:51 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// Heredoc signal handler - aborts heredoc on SIGINT
// heredoc redirects input from a temporary file that has lines of text,
// used with ( << ) oeprator

static void	heredoc_child_process(char *delimiter)
{
	int		fd;
	char	*line;
	char	*tmp_file;

	tmp_file = "/tmp/minishell_heredoc";
	setup_heredoc_signals();
	fd = open(tmp_file, O_WRONLY);
	if (fd == -1)
		exit(1);
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			close(fd);
			exit(0);
		}
		ft_putendl_fd(line, fd);
		free(line);
	}
}

int	create_heredoc_file(char *delimiter)
{
	int		fd;
	char	*tmp_file;
	pid_t	pid;
	int		status;

	tmp_file = "/tmp/minishell_heredoc";
	fd = open(tmp_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
		return (-1);
	close(fd);
	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
		heredoc_child_process(delimiter);
	else
	{
		waitpid(pid, &status, 0);
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
			return (unlink(tmp_file), -1);
	}
	fd = open(tmp_file, O_RDONLY);
	unlink(tmp_file);
	return (fd);
}
