/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 21:43:19 by maborges          #+#    #+#             */
/*   Updated: 2025/10/14 13:07:27 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/* static int	is_valid_identifier(const char *s)
{
	while (*s)
	{
		if (!(ft_isalnum((unsigned char)*s) || *s == '_'
				|| !ft_isalpha((unsigned char) *s)))
			return (0);
		s++;
	}
	return (1);
} */

int	builtin_unset(char **args, t_mshell *shell)
{
	int	i;

	i = 0;
	if (!args[1])
		return (0);
	while (args[++i])
	{
		if (!is_valid_identifier(args[i]))
		{
			fprintf(stderr, "unset: `%s': not a valid identifier\n", args[i]);
			shell->exit_status = 1;
			continue ;
		}
		unset_env_var(shell, args[i]);
	}
	return (shell->exit_status);
}
