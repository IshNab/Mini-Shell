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
// set flags (like append node)
// create command structure that executor can use
//safe_malloc better bc checks for NULL, if fails- calls panic to exit program
static t_command	*init_command_node(int argc)
{
	t_command	*cmd;
	int			i;

	cmd = safe_malloc(sizeof(t_command));
	cmd->base.type = NODE_CMD;
	cmd->input_file = NULL;
	cmd->output_file = NULL;
	cmd->is_append = 0;
	cmd->heredoc_delimiter = NULL;
	cmd->args = safe_malloc(sizeof(char *) * (argc + 1));
	i = 0;
	while (i <= argc)
		cmd->args[i++] = NULL;
	return (cmd);
}

/*this is used by the process_token function in utils folder, but too many functions in that file already*/
/*handles token_word*/
int	process_word_token(t_command *cmd, t_token *token, int *i)
{
	char	*dup;

	dup = ft_strdup(token->value);
	if (!dup)
		return (0);
	cmd->args[(*i)++] = dup;
	return (1);
}

t_command	*create_command_node(t_token *tokens)
{
	t_command	*cmd;
	int			i;

	if (!tokens)
		return (NULL);
	cmd = init_command_node(count_word_tokens(tokens));
	i = 0;
	if (!process_tokens(cmd, tokens, &i))
		return (cleanup_command(cmd), NULL);
	cmd->args[i] = NULL;
	return (cmd);
}