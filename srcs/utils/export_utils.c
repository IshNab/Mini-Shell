/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 14:34:54 by maborges          #+#    #+#             */
/*   Updated: 2025/10/14 20:48:26 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	error_msg_export(char *id)
{
	ft_putstr_fd("minishell: export: ", 2);
	ft_putstr_fd(id, 2);
	ft_putstr_fd(": not a valid identifier\n", 2);
	return (1);
}

int	is_valid_identifier(char *s)
{
	char *start = s;

	if (!s || !*s)
		return (0);
	if ((!ft_isalpha(*s) && *s != '_'))
	{
        printf("DEBUG: First char '%c' is not alpha/underscore\n", *s);
		return (0);
	}
	s++;
	while (*s && *s != '=')
	{
		if ((!ft_isalnum(*s) && *s != '_'))
		{
			printf("DEBUG: invaalid char '%c' at position '%ld' in '%s'\n", *s, s - start, start);
			return (0);
		}
		s++;
	}
	return (1);
}

static void	swap_vars(t_env *a, t_env *b)
{
	char	*temp_key;
	char	*temp_value;

	temp_key = a->key;
	temp_value = a->value;
	a->key = b->key;
	a->value = b->value;
	b->key = temp_key;
	b->value = temp_value;
}

void	sort_vars_list(t_env *env)
{
	int		swapped;
	t_env	*current;
	t_env	*last_ptr;

	last_ptr = NULL;
	if (env == NULL || env->next == NULL)
		return ;
	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		current = env;
		while (current->next != last_ptr)
		{
			if (ft_strcmp(current->key, current->next->key) > 0)
			{
				swap_vars(current, current->next);
				swapped = 1;
			}
			current = current->next;
		}
		last_ptr = current;
	}
}
