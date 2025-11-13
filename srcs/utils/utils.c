/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 16:35:22 by maborges          #+#    #+#             */
/*   Updated: 2025/11/13 16:28:19 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	ft_envsize(t_env *env)
{
	t_env	*tmp;
	int		i;

	tmp = env;
	i = 0;
	while (tmp)
	{
		tmp = tmp->next;
		i++;
	}
	return (i);
}

void	*safe_malloc(size_t size)
{
	void	*memory;

	memory = malloc(size);
	if (!memory)
	{
		panic("malloc failed");
		return (NULL);
	}
	return (memory);
}

int	fork_wrapper(t_mshell *shell)
{
	int	pid;

	pid = fork();
	if (pid == -1)
	{
		ft_putstr_fd("fork failed: ", 2);
		ft_putstr_fd(strerror(errno), 2);
		ft_putstr_fd("\n", 2);
		shell->exit_status = 1;
	}
	return (pid);
}

void	dup2_wrapper(t_mshell *shell, int fd)
{
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("minishell: dup2");
		close(fd);
		shell->exit_status = 1;
		exit(1);
	}
}

void	print_banner(void)
{
	printf(
		GREEN"███╗   ███╗██╗███╗   ██╗██╗███████╗██╗  ██╗███████╗██╗     ██╗\n"
		"████╗ ████║██║████╗  ██║██║██╔════╝██║  ██║██╔════╝██║     ██║\n"
		"██╔████╔██║██║██╔██╗ ██║██║███████╗███████║█████╗  ██║     ██║\n"
		"██║╚██╔╝██║██║██║╚██╗██║██║╚════██║██╔══██║██╔══╝  ██║     ██║\n"
		"██║ ╚═╝ ██║██║██║ ╚████║██║███████║██║  ██║███████╗███████╗███████╗\n"
		"╚═╝     ╚═╝╚═╝╚═╝  ╚═══╝╚═╝╚══════╝╚═╝╚═╝╚══════╝
			╚══════╝╚══════╝\n\n"RESET);
}
