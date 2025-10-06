/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 16:16:33 by maborges          #+#    #+#             */
/*   Updated: 2025/10/06 20:26:23 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	parser(char *input, char **envp, t_mshell *shell)
{
	t_token	*tokens;
	int		expand;

	expand = 0;
	//error check? when to free the linked list?
	tokens = ms_tokenize(input);
	if (!tokens)
		return (0);
	if (!validate_syntax(tokens))
	{
		free_token_list(tokens);
		return (0);
	}
	//TODO a way to check if we need to expand or not (flags?)
	expand_vars(tokens, shell);
	build_simple_ast(tokens);

}
