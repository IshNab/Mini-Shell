/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 00:00:00 by inabakka          #+#    #+#             */
/*   Updated: 2025/10/04 21:17:49 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*get_env_value(const char *var, char **envp)
{
	int	k;
	int	len;

	k = 0;
	len = ft_strlen(var);
	while (envp && envp[k])
	{
		if (!ft_strncmp(envp[k], var, len) && envp[k][len] == '=')
			return (envp[k] + len + 1);
		k++;
	}
	return (NULL);
}

void	expand_token_loop(const char *token, char **res, char **envp, \
	int last_status);

char	*ms_expand_token(const char *token, char **envp, int last_status)
{
	char	*res;

	res = NULL;
	expand_token_loop(token, &res, envp, last_status);
	if (!res)
		return (ft_strdup(""));
	return (res);
}

char	*expand_status(int last_status)
{
	return (ft_itoa(last_status));
}
