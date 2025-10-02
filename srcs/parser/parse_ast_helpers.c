/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_ast_helpers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inabakka <inabakka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 13:45:00 by inabakka          #+#    #+#             */
/*   Updated: 2025/09/29 13:45:07 by inabakka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	fill_args_from_tokens(t_token *tokens, char **args, int *argc)
{
	t_token	*curr;

	curr = tokens;
	while (curr)
	{
		if (curr->type == TOKEN_WORD)
			args[(*argc)++] = ft_strdup(curr->value);
		curr = curr->next;
	}
}

t_ast	*build_simple_ast(t_token *tokens)
{
	t_ast	*cmd_node;
	int		argc;
	char	**args;

	cmd_node = malloc(sizeof(t_ast));
	if (!cmd_node)
		return (NULL);
	cmd_node->type = NODE_CMD;
	cmd_node->left = NULL;
	cmd_node->right = NULL;
	cmd_node->exit_status = 0;
	argc = 0;
	args = malloc(sizeof(char *) * 100);
	if (!args)
	{
		free(cmd_node);
		return (NULL);
	}
	fill_args_from_tokens(tokens, args, &argc);
	args[argc] = NULL;
	cmd_node->args = args;
	return (cmd_node);
}

void	execute_ast_wrapper(t_ast *ast)
{
	// For now, just print the command that would be executed
	// TODO: Implement proper execution
	if (ast && ast->args && ast->args[0])
	{
		printf("Would execute: ");
		int i = 0;
		while (ast->args[i])
		{
			printf("%s", ast->args[i]);
			if (ast->args[i + 1])
				printf(" ");
			i++;
		}
		printf("\n");
	}
}

void	parse_and_exec_ast(t_token *tokens)
{
	t_ast	*ast;

	ast = build_simple_ast(tokens);
	if (ast)
	{
		execute_ast_wrapper(ast);
		free_ast(ast);
	}
}
