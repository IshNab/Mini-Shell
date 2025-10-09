/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 16:11:59 by maborges          #+#    #+#             */
/*   Updated: 2025/10/08 19:02:05 by maborges         ###   ########.fr       */
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

	// Key not found, create new node
	new_node = malloc(sizeof(t_env));
	new_node->key = ft_strdup(key);
	new_node->value = ft_strdup(value);
	new_node->next = shell->env;
	shell->env = new_node;
}

static int	is_valid_identifier(const char *s)
{
	if (!s || !*s)
		return (0);
	if (!(ft_isalpha((unsigned char)*s) || *s == '_'))
		return (0);
	s++;
	while (*s && *s != '=')
	{
		if (!(ft_isalnum((unsigned char)*s) || *s == '_'))
			return (0);
		s++;
	}
	return (1);
}

int	builtin_export(char **args, t_mshell *shell)
{
	t_env	*current;
	int		i;
	int		status;
	char	*eq;

	if (!args[1])
	{
		current = shell->env;
		while (current)
		{
			printf("declare -x %s=\"%s\"\n", current->key, current->value);
			current = current->next;
		}
		return (0);
	}
	status = 0;
	i = 1;
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			fprintf(stderr, "export: `%s': not a valid identifier\n", args[i]); // TODO take out the fprintf()
			status = 1;
			i++;
			continue ;
		}
		eq = ft_strchr(args[i], '=');
		if (eq)
		{
			*eq = '\0';
			set_env_var(shell, args[i], eq + 1);
			*eq = '=';
		}
		else
		{
			if (!get_env_from_list(shell->env, args[i]))
				set_env_var(shell, args[i], "");
		}
		i++;
	}
	return (status);
}
