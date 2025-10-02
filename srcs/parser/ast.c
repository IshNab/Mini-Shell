/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inabakka <inabakka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 15:00:00 by inabakka          #+#    #+#             */
/*   Updated: 2025/09/24 15:04:40 by inabakka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

t_ast	*ast_new_node(t_node_type type, char *value)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (node == NULL)
		return (NULL);
	node->type = type;
	node->left = NULL;
	node->right = NULL;
	node->exit_status = 0;
	node->args = NULL;
	(void)value; // value parameter not used in current AST structure
	return (node);
}

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

void	print_ast(t_ast *node, int depth)
{
	int	i;
	int	j;

	if (node == NULL)
		return ;
	i = 0;
	while (i < depth)
	{
		printf("  ");
		i++;
	}
	printf("[Type: %d", node->type);
	if (node->args && node->args[0])
	{
		printf(", Args: ");
		j = 0;
		while (node->args[j])
		{
			printf("%s", node->args[j]);
			if (node->args[j + 1])
				printf(" ");
			j++;
		}
	}
	printf("]\n");
	print_ast(node->left, depth + 1);
	print_ast(node->right, depth + 1);
}
