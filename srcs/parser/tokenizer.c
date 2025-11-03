/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 00:00:00 by inabakka          #+#    #+#             */
/*   Updated: 2025/10/07 15:07:24 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	list_token_append(t_token *new, t_token **head, t_token **tail)
{
	if (!*head)
		*head = new;
	else
		(*tail)->next = new;
	*tail = new;
}

// // Check for special characters
// static void	check_special_chars(t_token *new, const char *input, int *i)
// {
// 	int	start;

// 	if (input[*i] == '|')
// 	{
// 		new->type = TOKEN_PIPE;
// 		new->value = ft_strdup("|");
// 		(*i)++;
// 	}
// 	else if (input[*i] == '<' && input[*i + 1] == '<')
// 	{
// 		new->type = TOKEN_HEREDOC;
// 		new->value = ft_strdup("<<");
// 		(*i) += 2;
// 	}
// 	else if (input[*i] == '<')
// 	{
// 		new->type = TOKEN_REDIR_IN;
// 		new->value = ft_strdup("<");
// 		(*i)++;
// 	}
// 	else if (input[*i] == '>' && input[*i + 1] == '>')
// 	{
// 		new->type = TOKEN_APPEND;
// 		new->value = ft_strdup(">>");
// 		(*i) += 2;
// 	}
// 	else if (input[*i] == '>')
// 	{
// 		new->type = TOKEN_REDIR_OUT;
// 		new->value = ft_strdup(">");
// 		(*i)++;
// 	}
// 	else if (input[*i] == '=')
// 	{
// 		new->type = TOKEN_WORD;
// 		new->value = ft_strdup("=");
// 		(*i)++;
// 	}
// 	else if (input[*i] == '"')
// 	{
// 		// Handle double quoted content
// 		start = *i + 1; // Skip opening quote
// 		(*i)++;
// 		while (input[*i] && input[*i] != '"')
// 			(*i)++;
// 		if (input[*i] == '"')
// 		{
// 			new->type = TOKEN_DQUOTE;	//double quoted content, to be expanded
// 			new->value = ft_substr(input, start, *i - start);
// 			if (!new->value)
// 				new->value = ft_strdup(""); // Handle empty quotes
// 			(*i)++; // Skip closing quote
// 		}
// 		else //for unclosed double quotes
// 		{
// 			// Unclosed quote - treat as word
// 			new->type = TOKEN_WORD;
// 			new->value = ft_substr(input, start, *i - start);
// 			if (!new->value)
// 				new->value = ft_strdup(""); // Handle empty quotes
// 		}
// 	}
// 	else if (input[*i] == '\'')
// 	{
// 		// Handle single quoted content
// 		start = *i + 1; // Skip opening quote
// 		(*i)++;
// 		while (input[*i] && input[*i] != '\'')
// 			(*i)++;
// 		if (input[*i] == '\'')
// 		{
// 			new->type = TOKEN_SQUOTE; //single quoted content, not expanded
// 			new->value = ft_substr(input, start, *i - start);
// 			if (!new->value)
// 				new->value = ft_strdup(""); // Handle empty quotes
// 			(*i)++; // Skip closing quote
// 		}
// 		else
// 		{
// 			// Unclosed single quote - treat as word
// 			new->type = TOKEN_WORD;
// 			new->value = ft_substr(input, start, *i - start);
// 			if (!new->value)
// 				new->value = ft_strdup(""); // Handle empty quotes
// 		}
// 	}
// 	else
// 	{
// 		// Regular word - stop at quote boundaries
// 		start = *i;
// 		while (input[*i] && input[*i] != ' ' && input[*i] != '\t'
// 			&& input[*i] != '|' && input[*i] != '<' && input[*i] != '>'
// 			&& input[*i] != '"' && input[*i] != '\'' && input[*i] != '=')
// 			(*i)++;
// 		new->type = TOKEN_WORD;
// 		new->value = ft_substr(input, start, *i - start);
// 	}
// }

static int	handle_redirections(t_token *new, const char *input, int *i)
{
	if (input[*i] == '<' && input[*i + 1] == '<')
	{
		new->type = TOKEN_HEREDOC;
		new->value = ft_strdup("<<");
		(*i) += 2;
		return (1);
	}
	else if (input[*i] == '<')
	{
		new->type = TOKEN_REDIR_IN;
		new->value = ft_strdup("<");
		(*i)++;
		return (1);
	}
	else if (input[*i] == '>' && input[*i + 1] == '>')
	{
		new->type = TOKEN_APPEND;
		new->value = ft_strdup(">>");
		(*i) += 2;
		return (1);
	}
	else if (input[*i] == '>')
	{
		new->type = TOKEN_REDIR_OUT;
		new->value = ft_strdup(">");
		(*i)++;
		return (1);
	}
	return (0);
}

static void	handle_double_quote(t_token *new, const char *input, int *i)
{
	int	start;

	start = *i + 1;	//skip opening qoute
	(*i)++;
	while (input[*i] && input[*i] != '"')
		(*i)++;
	if (input[*i] == '"')
	{
		new->type = TOKEN_DQUOTE;	//double quoted content, to be expanded
		new->value = ft_substr(input, start, *i - start);
		if (!new->value)
			new->value = ft_strdup("");	//handle empty quotes
		(*i)++;	//skip closing quote
	}
	else	//for unclosed double quotes, unclosed treated as a word
	{
		new->type = TOKEN_WORD;
		new->value = ft_substr(input, start, *i - start);
		if (!new->value)
			new->value = ft_strdup("");
	}
}

static void	handle_single_quote(t_token *new, const char *input, int *i)
{
	int	start;

	start = *i + 1;	//skip opening quote
	(*i)++;
	while (input[*i] && input[*i] != '\'')
		(*i)++;
	if (input[*i] == '\'')
	{
		new->type = TOKEN_SQUOTE;	//single quoted content, not expanded
		new->value = ft_substr(input, start, *i - start);
		if (!new->value)
			new->value = ft_strdup("");	//handle empty quotes
		(*i)++;	//skip closing quote
	}
	else	//for unclosed single quotes, unclosed treated as a word
	{
		new->type = TOKEN_WORD;
		new->value = ft_substr(input, start, *i - start);
		if (!new->value)
			new->value = ft_strdup("");
	}
}

static void	handle_regular_word(t_token *new, const char *input, int *i)
{
	int	start;	//regular word stops at qoute boundaries

	start = *i;
	while (input[*i] && input[*i] != ' ' && input[*i] != '\t'
		&& input[*i] != '|' && input[*i] != '<' && input[*i] != '>'
		&& input[*i] != '"' && input[*i] != '\'' && input[*i] != '=')
		(*i)++;
	new->type = TOKEN_WORD;
	new->value = ft_substr(input, start, *i - start);
}

t_token	*ms_tokenize(const char *input)	//convert input string into linked list of tokens
{
	t_token	*head;
	t_token	*tail;
	t_token	*new;
	int		i;

	head = NULL;
	tail = NULL;
	i = 0;
	while (input[i])	//initialization
	{
		while (input[i] && (input[i] == ' ' || input[i] == '\t'))
			i++;
		if (!input[i])
			break ;
		new = malloc(sizeof(t_token));
		if (!new)
		{
			free_token_list(head);
			return (NULL);
		}
		new->next = NULL;
		check_special_chars(new, input, &i);	//check for special characters; identify tokens
		list_token_append(new, &head, &tail);
	}
	return (head);
}
