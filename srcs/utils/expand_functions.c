/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_functions.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 00:00:00 by inabakka          #+#    #+#             */
/*   Updated: 2025/10/07 16:41:18 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*get_env_from_list(t_env *env, const char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

//removes TOKEN_SQUOTE and TOKEN_DQUOTE tokens from the linked list
//keeps the meaning for the variable expansion
void	remove_quote_tokens(t_token **tokens)
{
	t_token	*curr;
	t_token	*prev;
	t_token	*temp;

	curr = *tokens;
	prev = NULL;
	while (curr)
	{
		if (curr->type == TOKEN_SQUOTE || curr->type == TOKEN_DQUOTE)
		{
			// Remove this quote token
			if (prev)
				prev->next = curr->next;
			else
				*tokens = curr->next;  // Update head
			temp = curr;
			curr = curr->next;
			free(temp->value);
			free(temp);
		}
		else
		{
			prev = curr;
			curr = curr->next;
		}
	}
}

char	*expand_word(char *word, t_env *env, int exit_status, pid_t shell_pid)
{
	char	*result;
	char	*temp;
	char	*value;
	int		i;
	int		j;

	// Simplified expansion - expand $VAR and $?
	result = ft_strdup("");
	i = 0;

	while (word[i])
	{
		if (word[i] == '$' && word[i + 1])
		{
			if (word[i + 1] == '?')
			{
				temp = ft_itoa(exit_status);
				result = str_append(result, temp);
				free(temp);
				i += 2;
			}
			else if (word[i + 1] == '$')
			{
				temp = ft_itoa(shell_pid);
				result = str_append(result, temp);
				free(temp);
				i += 2;
			}
			else if (ft_isalpha(word[i + 1]) || word[i + 1] == '_')
			{
				j = i + 1;
				while (word[j] && (ft_isalnum(word[j]) || word[j] == '_'))
					j++;
				temp = ft_substr(word, i + 1, j - i - 1);
				value = get_env_from_list(env, temp);
				if (value)
					result = str_append(result, value);
				free(temp);
				i = j;
			}
			else
			{
				temp = ft_substr(word, i, 1);
				result = str_append(result, temp);
				free(temp);
				i++;
			}
		}
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

void	expand_vars(t_token *tokens, t_mshell *shell)
{
	t_token	*curr;
	char	*expanded;
	int		in_single_quote;
	int		in_double_quote;

	curr = tokens;
	in_single_quote = 0;
	in_double_quote = 0;

	while (curr)
	{
		// Track quote state from separate quote tokens
		if (curr->type == TOKEN_SQUOTE)
			in_single_quote = !in_single_quote;
		else if (curr->type == TOKEN_DQUOTE)
			in_double_quote = !in_double_quote;

		// Only expand WORD tokens that are:
		// 1. NOT in single quotes (single quotes = no expansion)
		// 2. Either unquoted OR in double quotes (double quotes = yes expansion)
		else if (curr->type == TOKEN_WORD)
		{
			if (!in_single_quote)  // Expand if NOT in single quotes
			{
				expanded = expand_word(curr->value, shell->env, shell->exit_status, shell->shell_pid);
				free(curr->value);
				curr->value = expanded;
			}
			// If in_single_quote = 1, skip expansion (keep literal $HOME)
		}

		curr = curr->next;
	}
}


