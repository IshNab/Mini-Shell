/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 15:00:00 by inabakka          #+#    #+#             */
/*   Updated: 2025/10/14 20:39:10 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_ast	*create_pipe_node(t_ast *left, t_ast *right)
{
	t_pipeline	*pipe;

	pipe = malloc(sizeof(t_pipeline));
	if (!pipe)
		return (NULL);
	pipe->base.type = NODE_PIPE;
	pipe->left = left;
	pipe->right = right;
	return ((t_ast *)pipe);
}

static t_token	*split_at_pipe(t_token *tokens, t_token *pipe)
{
	t_token	*curr;
	t_token	*right_tokens;

	curr = tokens;
	while (curr && curr->next != pipe)
		curr = curr->next;
	if (curr)
		curr->next = NULL;
	right_tokens = pipe->next;
	free(pipe->value);
	free(pipe);
	return (right_tokens);
}

static t_token	*find_pipe(t_token *tokens)
{
	while (tokens)
	{
		if (tokens->type == TOKEN_PIPE)
			return (tokens);
		tokens = tokens->next;
	}
	return (NULL);
}

t_ast	*build_ast(t_token *tokens)
{
	t_token		*pipe;
	t_token		*right_tokens;
	t_ast		*left;
	t_ast		*right;

	if (!tokens)
		return (NULL);
	pipe = find_pipe(tokens);
	if (pipe)
	{
		right_tokens = split_at_pipe(tokens, pipe);
		left = build_ast(tokens);
		right = build_ast(right_tokens);
		return (create_pipe_node(left, right));
	}
	else
	{
		return ((t_ast *)create_command_node(tokens));
	}
}



