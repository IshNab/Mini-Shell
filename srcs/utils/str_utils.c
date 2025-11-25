/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 00:00:00 by inabakka          #+#    #+#             */
/*   Updated: 2025/11/20 13:10:02 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*str_append(char *s1, const char *s2)
{
	char	*new_s;
	int		len1;
	int		len2;
	int		i;
	int		j;

	if (!s1 || !s2)
		return (NULL);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	new_s = (char *)malloc(sizeof(char) * (len1 + len2 + 1));
	if (!new_s)
	{
		free(s1);
		return (NULL);
	}
	i = -1;
	while (++i < len1)
		new_s[i] = s1[i];
	j = -1;
	while (++j < len2)
		new_s[i + j] = s2[j];
	new_s[i + j] = '\0';
	free(s1);
	return (new_s);
}

char	**env_to_array(t_env *env)
{
	char	**env_array;
	t_env	*current;
	int		i;
	int		count;
	char	*tmp;

	count = 0;
	current = env;
	while (current)
	{
		if (current->exported)
			count++;
		current = current->next;
	}
	env_array = safe_malloc(sizeof(char *) * (count + 1));
	current = env;
	i = 0;
	tmp = NULL;
	while (current)
	{
		if (current->exported)
		{
			tmp = ft_strjoin(current->key, "=");
			env_array[i] = ft_strjoin(tmp, current->value);
			free(tmp);
			i++;
		}
		current = current->next;
	}
	env_array[i] = NULL;
	return (env_array);
}
