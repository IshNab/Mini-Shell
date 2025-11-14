/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   frees.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 20:37:37 by maborges          #+#    #+#             */
/*   Updated: 2025/11/13 17:38:20 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	free_cmd(t_command *cmd)
{
	int	i;

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
}

void	free_ast(t_ast *node)
{
	t_command	*cmd;
	t_pipeline	*pipe;

	cmd = NULL;
	if (node == NULL)
		return ;
	if (node->type == NODE_PIPE)
	{
		pipe = (t_pipeline *)node;
		free_ast(pipe->left);
		free_ast(pipe->right);
		free(pipe);
	}
	else if (node->type == NODE_CMD)
	{
		cmd = (t_command *)node;
		free_cmd(cmd);
		free(cmd);
	}
}

void	free_env_array(char **env_array)
{
	int	i;

	i = 0;
	while (env_array[i])
		free(env_array[i++]);
	free(env_array);
}

void	cleanup_shell(t_mshell *shell)
{
	t_env	*current;
	t_env	*next;

	current = shell->env;
	while (current)
	{
		next = current->next;
		free(current->key);
		free(current->value);
		free(current);
		current = next;
	}
}
