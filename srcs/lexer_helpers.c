/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inabakka <inabakka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 12:20:00 by inabakka          #+#    #+#             */
/*   Updated: 2025/09/23 12:01:11 by inabakka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/lexer.h"
#include "../inc/minishell.h"

typedef struct s_expand_ctx
{
	char	**res;
	char	**envp;
	int		last_status;
}	t_expand_ctx;

char	*expand_var(const char *token, int *i, char **envp)
{
	char	var[128];
	int		j;
	int		len;
	char	*val;

	j = *i + 1;
	while (ft_isalnum(token[j]) || token[j] == '_')
		j++;
	len = j - *i - 1;
	ft_strlcpy(var, token + *i + 1, len + 1);
	val = get_env_value(var, envp);
	*i = j - 1;
	if (val)
		return (ft_strdup(val));
	return (ft_strdup(""));
}

static void	expand_token_handle_dollar(
	const char *token,
	t_expand_ctx *ctx,
	int *i)
{
	char	*tmp;

	if (token[*i + 1] == '?')
	{
		tmp = expand_status(ctx->last_status);
		*(ctx->res) = ft_strjoin_free(*(ctx->res), tmp);
		free(tmp);
		(*i)++;
	}
	else if (ft_isalnum(token[*i + 1])
		|| token[*i + 1] == '_')
	{
		tmp = expand_var(token, i, ctx->envp);
		*(ctx->res) = ft_strjoin_free(*(ctx->res), tmp);
		free(tmp);
	}
	else
		*(ctx->res) = str_append(*(ctx->res), '$');
}

static void	expand_token_loop(const char *token, t_expand_ctx *ctx)
{
	int	in_squote;
	int	in_dquote;
	int	i;

	in_squote = 0;
	in_dquote = 0;
	i = 0;
	while (token[i])
	{
		if (token[i] == '\'' && !in_dquote)
			in_squote = !in_squote;
		else if (token[i] == '"' && !in_squote)
			in_dquote = !in_dquote;
		else if (token[i] == '$' && !in_squote)
			expand_token_handle_dollar(token, ctx, &i);
		else
			*(ctx->res) = str_append(*(ctx->res), token[i]);
		i++;
	}
}

char	*ms_expand_token(const char *token, char **envp, int last_status)
{
	t_expand_ctx	ctx;
	char			*res;

	res = NULL;
	ctx.res = &res;
	ctx.envp = envp;
	ctx.last_status = last_status;
	expand_token_loop(token, &ctx);
	if (!res)
		return (ft_strdup(""));
	return (res);
}
