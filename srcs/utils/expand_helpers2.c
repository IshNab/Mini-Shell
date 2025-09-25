/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_helpers2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inabakka <inabakka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 00:00:00 by inabakka          #+#    #+#             */
/*   Updated: 2025/09/24 14:13:05 by inabakka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"
#include <stdlib.h>
#include <string.h>

void	init_expand_vars(t_expand_vars *vars)
{
	vars->buf[0] = '\0';
	vars->j = 0;
	vars->var[0] = '\0';
	vars->len = 0;
	vars->val = NULL;
}

void	expand_token_handle(char c, t_expand_ctx *ctx)
{
	if (c == '\'' && !(ctx->st.in_dquote))
		ctx->st.in_squote = !(ctx->st.in_squote);
	else if (c == '"' && !(ctx->st.in_squote))
		ctx->st.in_dquote = !(ctx->st.in_dquote);
}

void	expand_token_handle_dollar(const char *token, t_expand_ctx *ctx)
{
	char	*tmp;

	tmp = expand_variable(token, &(ctx->st.i), ctx->envp, ctx->last_status);
	if (tmp)
		expand_and_append(ctx->res, tmp);
}

int	is_quote_to_skip(char c, int in_dquote, int in_squote)
{
	if ((c == '\'' && !in_dquote) || (c == '"' && !in_squote))
		return (1);
	return (0);
}
