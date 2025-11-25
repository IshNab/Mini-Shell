/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inabakka <inabakka@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 14:01:49 by inabakka          #+#    #+#             */
/*   Updated: 2025/11/25 14:15:20 by inabakka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	ends_with_equal(char *str)
{
	int	len;

	if (!str)
		return (0);
	len = ft_strlen(str);
	return (len > 0 && str[len - 1] == '=');
}

char	*merge_split_assignment(char **args, int *idx)
{
	char	*result;
	char	*tmp;
	int		i;

	result = ft_strdup(args[*idx]);
	i = *idx + 1;
	while (args[i] && !ft_strchr(args[i], '='))
	{
		tmp = result;
		result = ft_strjoin(result, " ");
		free(tmp);
		tmp = result;
		result = ft_strjoin(result, args[i]);
		free(tmp);
		i++;
	}
	*idx = i - 1;
	return (result);
}