/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_builder.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 14:35:26 by maborges          #+#    #+#             */
/*   Updated: 2025/10/08 13:56:50 by maborges         ###   ########.fr       */
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

// Create a command node (with redirections)
t_command	*create_command_node(t_token *tokens)
{
	t_command	*cmd;
	int			argc;
	int			i;

	if (!tokens)
		return (NULL);
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
	while (tokens)
	{
		if (tokens->type == TOKEN_WORD)
		{
			cmd->base.args[i++] = ft_strdup(tokens->value);
			tokens = tokens->next;
		}
		else if (tokens->type == TOKEN_REDIR_IN)
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
		else if (tokens->type == TOKEN_APPEND)
		{
			tokens = tokens->next;
			if (tokens && tokens->type == TOKEN_WORD)
			{
				cmd->output_file = ft_strdup(tokens->value);
				cmd->is_append = 1;
				tokens = tokens->next;
			}
		}
		else if (tokens->type == TOKEN_HEREDOC)
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
