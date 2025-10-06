/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 00:00:00 by inabakka          #+#    #+#             */
/*   Updated: 2025/10/05 16:22:01 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*char	*ft_strtok(char *str, const char *delim)
{
	static char	*save_ptr = NULL;
	char		*token_start;

	if (str)
		save_ptr = str;
	if (!save_ptr || *save_ptr == '\0')
		return (NULL);
	while (*save_ptr && strchr(delim, *save_ptr))
		save_ptr++;
	if (*save_ptr == '\0')
		return (NULL);
	token_start = save_ptr;
	while (*save_ptr && !strchr(delim, *save_ptr))
		save_ptr++;
	if (*save_ptr)
	{
		*save_ptr = '\0';
		save_ptr++;
	}
	else
		save_ptr = NULL;
	return (token_start);
} */

/* char	*ms_tokenize_next(const char *input, int *i, int *in_squote,
			int *in_dquote)
{
	char	*token;
	int		start;
	int		len;

	if (!input[*i])
		return (NULL);
	start = *i;
	len = 0;
	// Handle quoted content
	if (input[*i] == '\'' && !(*in_dquote))
	{
		*in_squote = !(*in_squote);
		(*i)++;
		token = ft_strdup("'");
		return (token);
	}
	else if (input[*i] == '"' && !(*in_squote))
	{
		*in_dquote = !(*in_dquote);
		(*i)++;
		token = ft_strdup("\"");
		return (token);
	}
	// Handle quoted word content
	else if (*in_squote || *in_dquote)
	{
		while (input[*i] && ((*in_squote && input[*i] != '\'') ||
			   (*in_dquote && input[*i] != '"')))
		{
			(*i)++;
			len++;
		}
		token = ft_strndup(&input[start], len);
		return (token);
	}
	// Handle regular word
	else
	{
		while (input[*i] && input[*i] != ' ' && input[*i] != '\t' &&
			   input[*i] != '\'' && input[*i] != '"' &&
			   input[*i] != '|' && input[*i] != '<' && input[*i] != '>')
		{
			(*i)++;
			len++;
		}
		if (len == 0)
			return (NULL);
		token = ft_strndup(&input[start], len);
		return (token);
	}
} */

static void	list_token_append(t_token *new, t_token *head, t_token *tail)
{
	if (!head)
		head = new;
	else
		tail->next = new;
	tail = new;

	/* if (token)
	{
		tokens[*t] = token;
		(*t)++;
	} */
}

/* static void	init_tokenize_vars(int *i, int *in_squote, int *in_dquote)
{
	*i = 0;
	*in_squote = 0;
	*in_dquote = 0;
} */

// Check for special characters
static void	check_special_chars(t_token *new, const char *input, int *i)
{
	if (input[*i] == '|')
	{
		new->type = TOKEN_PIPE;
		new->value = ft_strdup("|");
		(*i)++;
	}
	else if (input[*i] == '<')
	{
		new->type = TOKEN_REDIR_IN;
		new->value = ft_strdup("<");
		(*i)++;
	}
	else if (input[*i] == '>' && input[*i + 1] == '>')
	{
		new->type = TOKEN_APPEND;
		new->value = ft_strdup(">>");
		(*i) += 2;
	}
	else if (input[*i] == '>')
	{
		new->type = TOKEN_REDIR_OUT;
		new->value = ft_strdup(">");
		(*i)++;
	}
	else if (input[*i] == '"' || input[*i] == '\'')
	{
		// Handle quoted string
		char quote = input[*i++];
		int start = i;
		while (input[*i] && input[*i] != quote)
			(*i)++;
		new->type = TOKEN_WORD;
		new->value = ft_substr(input, start, i - start);
		if (input[*i] == quote)
			(*i)++;
	}
	else
	{
		// Regular word
		int start = i;
		while (input[*i] && input[*i] != ' ' && input[*i] != '\t'
			&& input[*i] != '|' && input[*i] != '<' && input[*i] != '>')
			(*i)++;
		new->type = TOKEN_WORD;
		new->value = ft_substr(input, start, *i - start);
	}
}

char	**ms_tokenize(const char *input)
{
	t_token	*head;
	t_token	*tail;
	t_token	*new;
	int		i;
	char	*token;

	head = NULL;
	tail = NULL;
	i = 0;
	while (input[i])
	{
		while (input[i] && (input[i] == ' ' || input[i] == '\t'))
			i++;
		if (!input[i])
			break ;
		new = malloc(sizeof(t_token));
		new->next = NULL;
		check_special_chars(&new, &input, &i);
		new->value = token;
		list_token_append(&new, &head, &tail);
	}
	return (head);
}
