/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inabakka <inabakka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 14:05:22 by maborges          #+#    #+#             */
/*   Updated: 2025/09/23 15:14:35 by maborges         ###   ########.fr       */
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
# include <signal.h>
# include "../libraries/libft.h"
# include "../libraries/ft_printf.h"

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
//								Structs                                        /
//=============================================================================/
typedef struct s_cmd
{
	char			**args;
	char			*input_file;
	char			*output_file;
	int				append; //0 no, 1 yes
	struct s_cmd	*next;
}	t_cmd;


//=============================================================================/
//								Parser                                         /
//=============================================================================/

int				parse_command(char *input, char **envp);

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

//=============================================================================/
//								Executor                                       /
//=============================================================================/

int				exec_cmd(t_cmd *cmd, char **envp);

//=============================================================================/
//								Utils                                          /
//=============================================================================/

void			print_banner(void);
void			signal_handler(int sig);
void			setup_signals(void);
int				panic(char *error_msg);
int				fork_wrapper(void);

// Tokenizer for minishell
char			**ms_tokenize(const char *input);
char			*ms_remove_quotes(const char *token);
char			*ms_expand_token(const char *token, char **envp, int last_status);
char			*ft_strtok(char *str, const char *delim);

#endif
