/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_builder.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 14:35:26 by maborges          #+#    #+#             */
/*   Updated: 2025/10/07 16:03:18 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

- t_command *create_command_node(t_token *tokens)
- static int count_word_tokens(t_token *tokens)

// Create a command node (with redirections)
t_command	*create_command_node(t_token *tokens)
{
    t_command	*cmd;
    int			argc;
    int			i;

    cmd = malloc(sizeof(t_command));
    if (!cmd)
        return (NULL);

    // Initialize the base AST fields
    cmd->base.type = NODE_CMD;
    cmd->base.left = NULL;
    cmd->base.right = NULL;
    cmd->base.exit_status = 0;

    // Initialize command-specific fields
    cmd->input_file = NULL;
    cmd->output_file = NULL;
    cmd->is_append = 0;
    cmd->heredoc_delimiter = NULL;

    // Count and allocate args
    argc = count_word_tokens(tokens);
    cmd->base.args = malloc(sizeof(char *) * (argc + 1));
    if (!cmd->base.args)
    {
        free(cmd);
        return (NULL);
    }

    // Fill args and handle redirections
    i = 0;
    while (tokens)
    {
        if (tokens->type == TOKEN_WORD)
            cmd->base.args[i++] = ft_strdup(tokens->value);
        else if (tokens->type == TOKEN_REDIR_IN)
        {
            tokens = tokens->next;
            if (tokens)
                cmd->input_file = ft_strdup(tokens->value);
        }
        else if (tokens->type == TOKEN_REDIR_OUT)
        {
            tokens = tokens->next;
            if (tokens)
                cmd->output_file = ft_strdup(tokens->value);
        }
        else if (tokens->type == TOKEN_APPEND)
        {
            tokens = tokens->next;
            if (tokens)
            {
                cmd->output_file = ft_strdup(tokens->value);
                cmd->is_append = 1;
            }
        }
        else if (tokens->type == TOKEN_HEREDOC)
        {
            tokens = tokens->next;
            if (tokens)
                cmd->heredoc_delimiter = ft_strdup(tokens->value);
        }

        tokens = tokens->next;
    }
    cmd->base.args[i] = NULL;

    return (cmd);
}
