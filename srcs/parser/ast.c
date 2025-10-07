/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 15:00:00 by inabakka          #+#    #+#             */
/*   Updated: 2025/10/07 16:02:05 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	free_ast(t_ast *node)
{
	int	i;

	if (node == NULL)
		return ;
	if (node->args)
	{
		i = 0;
		while (node->args[i])
		{
			free(node->args[i]);
			i++;
		}
		free(node->args);
	}
	free_ast(node->left);
	free_ast(node->right);
	free(node);
}

// Create a pipe node (RECURSIVE!)
t_ast	*create_pipe_node(t_ast *left, t_ast *right)
{
    t_ast	*pipe;

    pipe = malloc(sizeof(t_ast));
    if (!pipe)
        return (NULL);

    pipe->type = NODE_PIPE;
    pipe->left = left;   // Left side of pipe (recursive!)
    pipe->right = right; // Right side of pipe (recursive!)
    pipe->args = NULL;
    pipe->exit_status = 0;

    return (pipe);
}

// Split token list at pipe
static t_token	*split_at_pipe(t_token *tokens, t_token *pipe)
{
    t_token	*curr;

    curr = tokens;
    while (curr && curr->next != pipe)
        curr = curr->next;

    if (curr)
        curr->next = NULL;  // Cut the list

    return (pipe->next);  // Return right side
}

// Find pipe token in list
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

// RECURSIVE AST builder
t_ast	*build_ast(t_token *tokens)
{
    t_token		*pipe;
    t_token		*right_tokens;
    t_ast		*left;
    t_ast		*right;

    if (!tokens)
        return (NULL);

    // Find first pipe (if any)
    pipe = find_pipe(tokens);

    if (pipe)
    {
        // We have a pipe - RECURSIVE CASE
        right_tokens = split_at_pipe(tokens, pipe);

        // Build left side (RECURSIVE!)
        left = build_ast(tokens);

        // Build right side (RECURSIVE!)
        right = build_ast(right_tokens);

        // Create pipe node connecting them
        return (create_pipe_node(left, right));
    }
    else
    {
        // No pipe - BASE CASE
        // Create a command node (cast to t_ast* for return)
        return ((t_ast *)create_command_node(tokens));
    }
}



