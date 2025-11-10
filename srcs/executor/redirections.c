/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 13:19:42 by maborges          #+#    #+#             */
/*   Updated: 2025/11/08 19:22:14 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	when_input_file(t_command *cmd, int *saved_stdin, int fd)
{
	fd = open(cmd->input_file, O_RDONLY);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->input_file, 2);
		ft_putstr_fd(": ", 2);
		perror("");
		return (-1);
	}
	*saved_stdin = dup(STDIN_FILENO);
	if (*saved_stdin == -1)
		return (close(fd), -1);
	if (dup2(fd, STDIN_FILENO) == -1)
		return (close(fd), close(*saved_stdin), -1);
	close(fd);
	return (0);
}

int	handle_input_redir(t_command *cmd, int *saved_stdin)
{
	int	fd;

	fd = 0;
	if (cmd->heredoc_delimiter)
	{
		fd = create_heredoc_file(cmd->heredoc_delimiter);
		if (fd == -1)
			return (-1);
		*saved_stdin = dup(STDIN_FILENO);
		if (*saved_stdin == -1)
			return (close(fd), -1);
		if (dup2(fd, STDIN_FILENO) == -1)
			return (close(fd), close(*saved_stdin), -1);
		close(fd);
	}
	else if (cmd->input_file)
		return (when_input_file(cmd, saved_stdin, fd));
	return (0);
}

int	handle_output_redir(t_command *cmd, int *saved_stdout)
{
	int	fd;
	int	flags;

	if (!cmd->output_file)
		return (0);
	flags = O_WRONLY | O_CREAT;
	if (cmd->is_append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(cmd->output_file, flags, 0644);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->output_file, 2);
		ft_putstr_fd(": ", 2);
		perror("");
		return (-1);
	}
	*saved_stdout = dup(STDOUT_FILENO);
	if (*saved_stdout == -1)
		return (close(fd), -1);
	if (dup2(fd, STDOUT_FILENO) == -1)
		return (close(fd), close(*saved_stdout), -1);
	return (close(fd), 0);
}

int	setup_redirections(t_command *cmd, int *saved_stdin, int *saved_stdout)
{
	*saved_stdin = -1;
	*saved_stdout = -1;

	if (handle_input_redir(cmd, saved_stdin) == -1)
		return (-1);
	if (handle_output_redir(cmd, saved_stdout) == -1)
	{
		if (*saved_stdin != -1)
		{
			dup2(*saved_stdin, STDIN_FILENO);
			close(*saved_stdin);
		}
		return (-1);
	}
	return (0);
}

void	restore_redirections(int saved_stdin, int saved_stdout)
{
	if (saved_stdin != -1)
	{
		dup2(saved_stdin, STDIN_FILENO);
		close(saved_stdin);
	}
	if (saved_stdout != -1)
	{
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdout);
	}
}
