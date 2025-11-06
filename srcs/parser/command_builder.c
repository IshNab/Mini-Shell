/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_builder.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 14:35:26 by maborges          #+#    #+#             */
/*   Updated: 2025/10/14 20:21:41 by maborges         ###   ########.fr       */
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

// Helper function to clean up partially allocated command; for ft_strdup functions
static void	cleanup_command(t_command *cmd)
{
	int	i;

	if (!cmd)
		return ;
	if (cmd->args)
	{
		i = 0;
		while (cmd->args[i])
		{
			free(cmd->args[i]);
			i++;
		}
		free(cmd->args);
	}
	if (cmd->input_file)
		free(cmd->input_file);
	if (cmd->output_file)
		free(cmd->output_file);
	if (cmd->heredoc_delimiter)
		free(cmd->heredoc_delimiter);
	free(cmd);
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
	char		*dup; //error check for ft_strdup

	if (!tokens)
		return (NULL);
	cmd = safe_malloc(sizeof(t_command));
	cmd->base.type = NODE_CMD;
	cmd->input_file = NULL;
	cmd->output_file = NULL;
	cmd->is_append = 0;
	cmd->heredoc_delimiter = NULL;
	argc = count_word_tokens(tokens);
	cmd->args = safe_malloc(sizeof(char *) * (argc + 1));	//safe_malloc better bc checks for NULL, if fails- calls panic to exit program
	i = 0;
	while (i <= argc)	//initialize all args in the array with NULL
		cmd->args[i++] = NULL;
	i = 0;	//reset i for the next loop
	while (tokens)	//iterate through the tokens, process each token
	{
		if (tokens->type == TOKEN_WORD)
		{
			dup = ft_strdup(tokens->value);
			if (!dup)
				return (cleanup_command(cmd), NULL)
			cmd->args[i++]	= dup;
			tokens = tokens->next;
		}
		else if (tokens->type == TOKEN_REDIR_IN)
		{
			tokens = tokens->next;
			if (tokens && tokens->type == TOKEN_WORD)
			{
				if (cmd->input_file)  // Free old value if exists
					free(cmd->input_file);
				dup = ft_strdup(tokens->value);
				if (!dup)
					return (cleanup_command(cmd), NULL);
				cmd->input_file = dup;
				tokens = tokens->next;
			}
		}
		else if (tokens->type == TOKEN_REDIR_OUT)
		{
			tokens = tokens->next;
			if (tokens && tokens->type == TOKEN_WORD)
			{
				if (cmd->output_file)  // Free old value if exists
					free(cmd->output_file);
				dup = ft_strdup(tokens->value);
				if (!dup)
					return (cleanup_command(cmd), NULL);
				cmd->output_file = dup;
				cmd->is_append = 0;	//reset append flag for > redirection, redir_out overwrites token_appedend 
				tokens = tokens->next;
			}
		}
		else if (tokens->type == TOKEN_APPEND)	// (>>) appends to the file instead of overwriting
		{
			tokens = tokens->next;
			if (tokens && tokens->type == TOKEN_WORD)
			{
				if (cmd->output_file)  // Free old value if exists
					free(cmd->output_file);
				dup = ft_strdup(tokens->value);
				if (!dup)
					return (cleanup_command(cmd), NULL);
				cmd->output_file = dup;
				cmd->is_append = 1;
				tokens = tokens->next;
			}
		}
		else if (tokens->type == TOKEN_HEREDOC)	// (<<) command reads from a file until it finds the delimiter
		{
			tokens = tokens->next;
			if (tokens && tokens->type == TOKEN_WORD)
			{
				if (cmd->heredoc_delimiter)  // Free old value if exists
					free(cmd->heredoc_delimiter);
				dup = ft_strdup(tokens->value);
				if (!dup)
					return (cleanup_command(cmd), NULL);
				cmd->heredoc_delimiter = dup;
				tokens = tokens->next;
			}
		}
		else
			tokens = tokens->next;  // Skip unknown tokens
	}
	cmd->args[i] = NULL;
	return (cmd);
}
