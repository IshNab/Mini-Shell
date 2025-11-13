/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_functions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 00:00:00 by inabakka          #+#    #+#             */
/*   Updated: 2025/11/13 16:30:52 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

//modified this because need access to shell's PID to expand $$
// Simplified expansion - expand $VAR and $?
static char	*append_and_advance(char *result, char *temp, int *i, int advance)
{
	result = str_append(result, temp);
	free(temp);
	*i += advance;
	return (result);
}

static char	*handle_dollar_expansion(char *word, char *result,
			int *i, t_mshell *shell)
{
	char	*temp;
	int		j;

	if (word[*i + 1] == '?')
		return (append_and_advance(result, ft_itoa(shell->exit_status), i, 2));
	else if (word[*i + 1] == '$')
		return (append_and_advance(result, ft_itoa(shell->shell_pid), i, 2));
	else if (word[*i + 1] == '@' || word[*i + 1] == '#'
		|| ft_isdigit(word[*i + 1]))
		return (*i += 2, result);
	else if (ft_isalpha(word[*i + 1]) || word[*i + 1] == '_')
	{
		j = *i + 1;
		while (word[j] && (ft_isalnum(word[j]) || word[j] == '_'))
			j++;
		temp = ft_substr(word, *i + 1, j - *i - 1);
		if (get_env_from_list(shell->env, temp))
			result = str_append(result, get_env_from_list(shell->env, temp));
		free(temp);
		*i = j;
		return (result);
	}
	return (append_and_advance(result, ft_substr(word, *i, 1), i, 1));
}

char	*exp_word(char *word, t_env *env, int exit_status, pid_t shell_pid)
{
	char	*result;
	char	*temp;
	int		i;

	result = ft_strdup("");
	i = 0;
	while (word[i])
	{
		if (word[i] == '$' && word[i + 1])
			result = handle_dollar_expansion(word, result, env, &i,
					exit_status, shell_pid);
		else
		{
			temp = ft_substr(word, i, 1);
			result = str_append(result, temp);
			free(temp);
			i++;
		}
	}
	return (result);
}
