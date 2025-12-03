/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_functions_2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 16:29:33 by maborges          #+#    #+#             */
/*   Updated: 2025/11/25 13:48:27 by maborges         ###   ########.fr       */
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
			if (prev)
				prev->next = curr->next;
			else
				*tokens = curr->next;
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

static int	skip_exp(t_token *tokens, t_token *curr)
{
	t_token	*tmp;

	tmp = tokens;
	while (tmp && tmp != curr)
	{
		if (tmp->type == TOKEN_PIPE)
			tmp = tmp->next;
		else if (tmp->type == TOKEN_WORD)
		{
			if (ft_strcmp(tmp->value, "export") == 0 && curr->value
				&& !ft_strchr(curr->value, '='))
				return (1);
			return (0);
		}
		else
			tmp = tmp->next;
	}
	return (0);
}

//expand variables then convert to WORD token
void	expand_vars(t_token *tokens, t_mshell *shell)
{
	t_token	*curr;
	char	*expd;

	curr = tokens;
	while (curr)
	{
		if (curr->type == TOKEN_DQUOTE)
		{
			expd = exp_word(curr->value, shell);
			free(curr->value);
			curr->value = expd;
			curr->type = TOKEN_WORD;
		}
		else if (curr->type == TOKEN_SQUOTE)
			curr->type = TOKEN_WORD;
		else if (curr->type == TOKEN_WORD && !skip_exp(tokens, curr))
		{
			expd = exp_word(curr->value, shell);
			free(curr->value);
			curr->value = expd;
		}
		curr = curr->next;
	}
}
