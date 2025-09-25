/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inabakka <inabakka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 00:00:00 by inabakka          #+#    #+#             */
/*   Updated: 2025/09/24 13:28:47 by inabakka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"
#include <stdlib.h>
#include <string.h>

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
	char	*buf;

	buf = (char *)malloc(16);
	if (!buf)
		return (NULL);
	ft_snprintf(buf, 16, "%d", last_status);
	return (buf);
}
