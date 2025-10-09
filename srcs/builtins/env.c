/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 14:28:47 by maborges          #+#    #+#             */
/*   Updated: 2025/10/08 12:52:06 by maborges         ###   ########.fr       */
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
		printf("%s=%s\n", current->key, current->value);
		current = current->next;
	}
	return (0);
}
