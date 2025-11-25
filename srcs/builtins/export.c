/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inabakka <inabakka@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 16:11:59 by maborges          #+#    #+#             */
/*   Updated: 2025/11/25 14:08:43 by inabakka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	set_env_var(t_mshell *shell, char *key, char *value)
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
			current->exported = 1;
			return ;
		}
		current = current->next;
	}
	new_node = malloc(sizeof(t_env));
	new_node->key = ft_strdup(key);
	new_node->value = ft_strdup(value);
	new_node->exported = 1;
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

static void	check_value_var(char *arg, t_mshell *shell)
{
	char	*eq;

	eq = ft_strchr(arg, '=');
	if (eq)
	{
		*eq = '\0';
		set_env_var(shell, arg, eq + 1);
		*eq = '=';
	}
	else
		if (!get_env_from_list(shell->env, arg))
			set_env_var(shell, arg, "");
}

static int	process_export_arg(char **args, int *i, t_mshell *shell)
{
	char	*full_arg;
	int		status;

	status = 0;
	if (ends_with_equal(args[*i]) && args[*i + 1])
	{
		full_arg = merge_split_assignment(args, i);
		if (!is_valid_identifier(full_arg))
			status = error_msg_export(full_arg);
		else
			check_value_var(full_arg, shell);
		free(full_arg);
	}
	else
	{
		if (!is_valid_identifier(args[*i]))
			status = error_msg_export(args[*i]);
		else
			check_value_var(args[*i], shell);
	}
	return (status);
}

int	builtin_export(char **args, t_mshell *shell)
{
	int	i;
	int	status;

	if (!args[1])
	{
		sort_vars_list(shell->env);
		print_envvars(shell);
		return (0);
	}
	status = 0;
	i = 1;
	while (args[i])
	{
		if (process_export_arg(args, &i, shell))
			status = 1;
		i++;
	}
	return (status);
}