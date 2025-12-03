/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 14:28:47 by maborges          #+#    #+#             */
/*   Updated: 2025/11/20 13:53:29 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static int	error_env(char *arg, int exit_code, t_mshell *shell)
{
	ft_putstr_fd("env: \'", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("\': No such file or directory", 2);
	ft_putstr_fd("\n", 2);
	if (shell)
		shell->exit_status = exit_code;
	return (exit_code);
}

int	builtin_env(char **args, t_mshell *shell)
{
	t_env	*current;

	if (args[1])
		return (error_env(args[1], 127, shell));
	current = shell->env;
	while (current)
	{
		if (current->exported)
			printf("%s=%s\n", current->key, current->value);
		current = current->next;
	}
	return (0);
}
