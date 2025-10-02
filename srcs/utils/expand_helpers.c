/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 00:00:00 by inabakka          #+#    #+#             */
/*   Updated: 2025/09/25 18:06:30 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

char	*expand_variable(const char *token, int *i,
			char **envp, int last_status);
void	init_expand_vars(t_expand_vars *vars);
void	expand_token_handle(char c, t_expand_ctx *ctx);
void	expand_token_handle_dollar(const char *token, t_expand_ctx *ctx);
int		is_quote_to_skip(char c, int in_dquote, int in_squote);

static char	*expand_variable_env(const char *token, int *i,
			char **envp, t_expand_vars *vars)
{
	vars->j = *i + 1;
	while (ft_isalnum(token[vars->j]) || token[vars->j] == '_')
		vars->j++;
	vars->len = vars->j - *i - 1;
	ft_strlcpy(vars->var, token + *i + 1, vars->len + 1);
	vars->val = get_env_value(vars->var, envp);
	*i = vars->j - 1;
	if (vars->val)
		return (ft_strdup(vars->val));
	return (NULL);
}

char	*expand_variable(const char *token, int *i,
			char **envp, int last_status)
{
	t_expand_vars	vars;
	char			*res;

	res = NULL;
	init_expand_vars(&vars);
	if (token[*i + 1] == '?')
	{
		ft_snprintf(vars.buf, sizeof(vars.buf), "%d", last_status);
		res = ft_strdup(vars.buf);
		(*i)++;
	}
	else if (ft_isalnum(token[*i + 1]) || token[*i + 1] == '_')
		res = expand_variable_env(token, i, envp, &vars);
	else
		res = str_append(NULL, '$');
	return (res);
}

void	expand_and_append(char **res, char *tmp)
{
	char	*old;

	old = *res;
	*res = ft_strjoin_free(*res, tmp);
	free(tmp);
	if (old)
		free(old);
}

static void	expand_token_loop_body(const char *token, t_expand_ctx *ctx)
{
	if (token[ctx->st.i] == '$' && !ctx->st.in_squote)
		expand_token_handle_dollar(token, ctx);
	else if (!is_quote_to_skip(token[ctx->st.i], ctx->st.in_dquote,
			ctx->st.in_squote))
		*(ctx->res) = str_append(*(ctx->res), token[ctx->st.i]);
}

void	expand_token_loop(const char *token, char **res, char **envp,
			int last_status)
{
	t_expand_ctx	ctx;

	ctx.st.in_squote = 0;
	ctx.st.in_dquote = 0;
	ctx.st.i = 0;
	ctx.envp = envp;
	ctx.last_status = last_status;
	ctx.res = res;
	while (token[ctx.st.i])
	{
		expand_token_handle(token[ctx.st.i], &ctx);
		expand_token_loop_body(token, &ctx);
		ctx.st.i++;
	}
}
