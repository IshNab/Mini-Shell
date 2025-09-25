/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inabakka <inabakka@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 16:01:02 by inabakka          #+#    #+#             */
/*   Updated: 2025/09/25 16:01:03 by inabakka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

//=============================================================================/
//								Library Headers                                /
//=============================================================================/

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include <string.h> //strerror
# include <readline/readline.h>
# include <readline/history.h>

# include "../libft/libft.h"

//=============================================================================/
//								Parser                                         /
//=============================================================================/

int		parse_command(char *input, char **envp);
char	*ms_remove_quotes(const char *token);
int		is_quote(char c);
char	*str_append(char *s, char c);
char	*expand_status(int last_status);
char	*ft_strtok(char *str, const char *delim);
char	**ms_tokenize(const char *input);
char	*ms_tokenize_next(const char *input, int *i, int *in_squote,int *in_dquote);
char	*ms_expand_token(const char *token, char **envp, int last_status)
void	expand_token_loop(const char *token, char **res, char **envp, \
	int last_status);
char	*expand_variable(const char *token, int *i, char **envp, int last_status);
void	expand_and_append(char **res, char *tmp);

#endif
