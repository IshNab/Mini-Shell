/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 21:43:19 by maborges          #+#    #+#             */
/*   Updated: 2025/11/11 16:17:20 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	error_msg_unset(char *id)
{
	ft_putstr_fd("minishell: unset: ", 2);
	ft_putstr_fd(id, 2);
	ft_putstr_fd(": not a valid identifier\n", 2);
	return (1);
}

void	unset_env_var(t_mshell *shell, const char *key)
{
	t_env	*current;
	t_env	*prev;

	current = shell->env;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				shell->env = current->next;
			free(current->key);
			free(current->value);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

int	builtin_unset(char **args, t_mshell *shell)
{
	int	i;
	int	status;

	i = 0;
	status = 0;
	if (!args[1])
		return (0);
	while (args[++i])
	{
		if (!is_valid_identifier(args[i]))
		{
			error_msg_unset(args[i]);
			status = 1;
			continue ;
		}
		unset_env_var(shell, args[i]);
	}
	shell->exit_status = status;
	return (status);
}
