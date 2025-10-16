/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 14:28:47 by maborges          #+#    #+#             */
/*   Updated: 2025/10/09 21:52:47 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	builtin_env(char **args, t_mshell *shell)
{
	t_env	*current;

	if (args[1])
		error_msg("env gets no args", 1, shell);
	current = shell->env;
	while (current)
	{
		printf("%s=%s\n", current->key, current->value);
		current = current->next;
	}
	return (0);
}
