/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_builder.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 14:35:26 by maborges          #+#    #+#             */
/*   Updated: 2025/10/07 16:48:25 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	count_word_tokens(t_token *tokens)
{
	int	count;

	count = 0;
	while (tokens)
	{
		if (tokens->type == TOKEN_WORD)
			count++;
		tokens = tokens->next;
	}
	return (count);
}

// extract command arguments and redirections from the tokens
// identify redirections and their target files
// set flags (liek append node)
// create command structure that executor can use
t_command	*create_command_node(t_token *tokens)
{
	t_command	*cmd;
	int			argc;
	int			i;
	//initialize command structure, NULL by default
	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->base.type = NODE_CMD;
	cmd->base.left = NULL;
	cmd->base.right = NULL;
	cmd->base.exit_status = 0;
	cmd->input_file = NULL;
	cmd->output_file = NULL;
	cmd->is_append = 0;
	cmd->heredoc_delimiter = NULL;
	argc = count_word_tokens(tokens);
	cmd->base.args = malloc(sizeof(char *) * (argc + 1));
	if (!cmd->base.args)
	{
		free(cmd);
		return (NULL);
	}
	i = 0;
	while (tokens)	//iterate through the tokens, process each token
	{
		if (tokens->type == TOKEN_WORD)
		{
			cmd->base.args[i++] = ft_strdup(tokens->value);	//add argument
			tokens = tokens->next;
		}
		else if (tokens->type == TOKEN_REDIR_IN)	// (<) redirects input from a file
		{
			tokens = tokens->next;
			if (tokens && tokens->type == TOKEN_WORD)
			{
				cmd->input_file = ft_strdup(tokens->value);
				tokens = tokens->next;
			}
		}
		else if (tokens->type == TOKEN_REDIR_OUT)
		{
			tokens = tokens->next;
			if (tokens && tokens->type == TOKEN_WORD)
			{
				cmd->output_file = ft_strdup(tokens->value);
				tokens = tokens->next;
			}
		}
		else if (tokens->type == TOKEN_APPEND)	// (>>) appends to the file instead of overwriting
		{
			tokens = tokens->next;
			if (tokens && tokens->type == TOKEN_WORD)
			{
				cmd->output_file = ft_strdup(tokens->value);
				cmd->is_append = 1;
				tokens = tokens->next;
			}
		}
		else if (tokens->type == TOKEN_HEREDOC)	// (<<) command reads from a file until it finds the delimiter
		{
			tokens = tokens->next;
			if (tokens && tokens->type == TOKEN_WORD)
			{
				cmd->heredoc_delimiter = ft_strdup(tokens->value);
				tokens = tokens->next;
			}
		}
		else
			tokens = tokens->next;  // Skip unknown tokens
	}
	cmd->base.args[i] = NULL;
	return (cmd);
}
