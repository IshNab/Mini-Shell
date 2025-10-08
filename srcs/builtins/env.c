/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 14:28:47 by maborges          #+#    #+#             */
/*   Updated: 2025/10/08 12:39:50 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	builtin_env(char **args, t_mshell *shell)
{
	t_env	*current;

	(void)args;
	if (args)
		error_msg("env gets no args", 1, shell);
	current = shell->env;
	while (current)
	{
		ft_putstr_fd(current->key, 1);
		ft_puchar_fd('=', 1);
		ft_putstr_fd(current->value, 1);
		ft_putchar_fd('\n', 1);
		current = current->next;
	}
	return (0);
}
