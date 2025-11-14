/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_builder_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 15:00:00 by inabakka          #+#    #+#             */
/*   Updated: 2025/11/14 16:05:43 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	handle_redir_in(t_command *cmd, t_token **tokens)
{
	char	*dup;

	*tokens = (*tokens)->next;
	if (*tokens && (*tokens)->type == TOKEN_WORD)
	{
		if (cmd->input_file)
			free(cmd->input_file);
		dup = ft_strdup((*tokens)->value);
		if (!dup)
			return (0);
		cmd->input_file = dup;
		*tokens = (*tokens)->next;
	}
	return (1);
}

/*handles token append when is_append = 1, redir_out when is_append = 0*/
static int	handle_redir_out(t_command *cmd, t_token **tokens, int is_append)
{
	char	*dup;

	*tokens = (*tokens)->next;
	if (*tokens && (*tokens)->type == TOKEN_WORD)
	{
		if (cmd->output_file)
			free(cmd->output_file);
		dup = ft_strdup((*tokens)->value);
		if (!dup)
			return (0);
		cmd->output_file = dup;
		cmd->is_append = is_append;
		*tokens = (*tokens)->next;
	}
	return (1);
}

static int	handle_heredoc(t_command *cmd, t_token **tokens)
{
	char	*dup;

	*tokens = (*tokens)->next;
	if (*tokens && (*tokens)->type == TOKEN_WORD)
	{
		if (cmd->heredoc_delimiter)
			free(cmd->heredoc_delimiter);
		dup = ft_strdup((*tokens)->value);
		if (!dup)
			return (0);
		cmd->heredoc_delimiter = dup;
		*tokens = (*tokens)->next;
	}
	return (1);
}

static int	handle_redirection(t_command *cmd, t_token **tokens)
{
	if ((*tokens)->type == TOKEN_REDIR_IN)
		return (handle_redir_in(cmd, tokens));
	else if ((*tokens)->type == TOKEN_REDIR_OUT)
		return (handle_redir_out(cmd, tokens, 0));
	else if ((*tokens)->type == TOKEN_APPEND)
		return (handle_redir_out(cmd, tokens, 1));
	else if ((*tokens)->type == TOKEN_HEREDOC)
		return (handle_heredoc(cmd, tokens));
	return (1);
}

int	process_tokens(t_command *cmd, t_token *tokens, int *i)
{
	while (tokens)
	{
		if (tokens->type == TOKEN_WORD)
		{
			if (!process_word_token(cmd, tokens, i))
				return (0);
			tokens = tokens->next;
		}
		else if (tokens->type >= TOKEN_REDIR_IN
			&& tokens->type <= TOKEN_HEREDOC)
		{
			if (!handle_redirection(cmd, &tokens))
				return (0);
		}
		else
			tokens = tokens->next;
	}
	return (1);
}
