/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 16:11:59 by maborges          #+#    #+#             */
/*   Updated: 2025/10/16 21:42:45 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	set_env_var(t_mshell *shell, const char *key, const char *value)
{
	t_env	*current;
	t_env	*new_node;

	current = shell->env;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			free(current->value);
			current->value = ft_strdup(value);
			return ;
		}
		current = current->next;
	}
	new_node = malloc(sizeof(t_env));
	new_node->key = ft_strdup(key);
	new_node->value = ft_strdup(value);
	new_node->next = shell->env;
	shell->env = new_node;
}

static void	print_envvars(t_mshell *shell)
{
	t_env	*current;

	current = shell->env;
	while (current)
	{
		printf("declare -x %s=\"%s\"\n", current->key, current->value);
		current = current->next;
	}
}

static void	check_value_var(char *arg)
{
	char	*eq;

	eq = ft_strchr(args, '=');
	if (eq)
	{
		*eq = '\0';
		set_env_var(shell, args, eq + 1);
		*eq = '=';
	}
	else
		if (!get_env_from_list(shell->env, args))
			set_env_var(shell, args, "");
}

int	builtin_export(char **args, t_mshell *shell)
{
	int		i;
	int		status;
	char	*eq;

	if (!args[1])
		return (sort_vars_list(shell->env), print_envvars(shell), 0);
	status = 0;
	i = 1;
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
			status = error_msg_export(args[i]);
		else
			check_value_var(args[i]);
		i++;
	}
	return (status);
}
