/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inabakka <inabakka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 14:05:22 by maborges          #+#    #+#             */
/*   Updated: 2025/09/23 11:42:45 by inabakka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include "../libraries/libft.h"
# include "../libraries/ft_printf.h"

//Colors

# define GREEN "\033[0;32m"
# define YELLOW "\033[0;33m"
# define CYAN "\033[0;36m"
# define RED "\033[1;31m"
# define RESET "\033[0m"

//UTILS

char	*ft_strtok(char *str, const char *delim);

// Tokenizer for minishell
char **ms_tokenize(const char *input);
char *ms_remove_quotes(const char *token);
char *ms_expand_token(const char *token, char **envp, int last_status);

void	print_banner(void);
void	signal_handler(int sig);
void	setup_signals(void);

#endif
