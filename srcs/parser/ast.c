/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 15:00:00 by inabakka          #+#    #+#             */
/*   Updated: 2025/10/07 16:36:30 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	free_ast(t_ast *node)
{
	int			i;
	t_command	*cmd;

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
	if (node->type == NODE_CMD)
	{
		cmd = (t_command *)node;
		if (cmd->input_file)
			free(cmd->input_file);
		if (cmd->output_file)
			free(cmd->output_file);
		if (cmd->heredoc_delimiter)
			free(cmd->heredoc_delimiter);
	}
	free_ast(node->left);
	free_ast(node->right);
	free(node);
}

t_ast	*create_pipe_node(t_ast *left, t_ast *right)
{
	t_ast	*pipe;

	pipe = malloc(sizeof(t_ast));
	if (!pipe)
		return (NULL);
	pipe->type = NODE_PIPE;
	pipe->left = left;
	pipe->right = right;
	pipe->args = NULL;
	pipe->exit_status = 0;
	return (pipe);
}

static t_token	*split_at_pipe(t_token *tokens, t_token *pipe)
{
	t_token	*curr;

	curr = tokens;
	while (curr && curr->next != pipe)
		curr = curr->next;
	if (curr)
		curr->next = NULL;
	return (pipe->next);
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
	pipe = find_pipe(tokens);	//look for pipes in the linked list
	if (pipe)
	{
		right_tokens = split_at_pipe(tokens, pipe);	//split the linked list at the pipe
		left = build_ast(tokens);	//recursively build the left side of the pipe
		right = build_ast(right_tokens);	//recursively build the right side of the pipe
		return (create_pipe_node(left, right));	//create a pipe node
	}
	else
	{
		return ((t_ast *)create_command_node(tokens));	//create a command node
	}
}



