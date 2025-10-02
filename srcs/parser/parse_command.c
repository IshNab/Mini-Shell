/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inabakka <inabakka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 14:35:26 by maborges          #+#    #+#             */
/*   Updated: 2025/09/29 13:53:39 by inabakka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	parse_command(char *input, char **envp)
{
	t_token	*tokens;

	(void)envp;
	tokens = lexer(input);
	if (!tokens)
		return (1);
	if (!validate_syntax(tokens))
	{
		free_token_list(tokens);
		return (1);
	}
	parse_and_exec_ast(tokens);
	free_token_list(tokens);
	return (0);
}
