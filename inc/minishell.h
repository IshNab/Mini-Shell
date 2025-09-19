/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 14:05:22 by maborges          #+#    #+#             */
/*   Updated: 2025/09/19 14:37:40 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

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
//								Colors                                         /
//=============================================================================/

# define GREEN "\033[0;32m"
# define YELLOW "\033[0;33m"
# define CYAN "\033[0;36m"
# define RED "\033[1;31m"
# define RESET "\033[0m"

//=============================================================================/
//								Utils                                          /
//=============================================================================/

void			print_banner(void);

//=============================================================================/
//								Parser                                         /
//=============================================================================/

int				parse_command(char *input);

//=============================================================================/
//								Builtins                                       /
//=============================================================================/

int				builtin_cd(char **args);
int				builtin_echo(char **args);
int				builtin_env(char **args);
int				builtin_exit(char **args);
int				builtin_export(char **args);
int				builtin_pwd(char **args);
int				builtin_unset(char **args);

#endif
