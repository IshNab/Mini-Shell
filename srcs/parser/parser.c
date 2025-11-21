/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 16:16:33 by maborges          #+#    #+#             */
/*   Updated: 2025/11/14 16:04:23 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	remove_empty_tokens(t_token **head)
{
    t_token	*curr;
    t_token	*prev;
    t_token	*tmp;

	if (!head || !*head)
		return ;
    prev = NULL;
	curr = *head;
    while (curr)
    {
        if (curr->value && curr->value[0] == '\0')
        {
            tmp = curr;
            if (prev)
                prev->next = curr->next;
            else
                *head = curr->next;
            curr = curr->next;
            if (tmp->value)
				free(tmp->value);
            free(tmp);
        }
        else
        {
            prev = curr;
            curr = curr->next;
        }
    }
}

static void normalize_quote_tokens(t_token *tokens)
{
	t_token *curr;

	curr = tokens;
	while (curr)
	{
		if (curr->type == TOKEN_DQUOTE || curr->type == TOKEN_SQUOTE)
			curr->type = TOKEN_WORD;
		curr = curr->next;
	}
}

t_ast	*parser(char *input, t_mshell *shell)
{
	t_token	*tokens;
	t_ast	*ast;

	if (!input || !*input)
		return (NULL);
	tokens = ms_tokenize(input);
	if (!tokens)
		return (NULL);
	expand_vars(tokens, shell);
	remove_empty_tokens(&tokens);
	normalize_quote_tokens(tokens);
	if (!validate_syntax(tokens))
	{
		free_token_list(tokens);
		shell->exit_status = 2;
		return (NULL);
	}
	ast = build_ast(tokens);
	if (!ast)
		return (free_token_list(tokens), NULL);
	free_token_list(tokens);
	return (ast);
}
