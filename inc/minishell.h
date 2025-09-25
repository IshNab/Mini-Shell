/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inabakka <inabakka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 14:05:22 by maborges          #+#    #+#             */
/*   Updated: 2025/09/24 15:51:58 by inabakka         ###   ########.fr       */
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
//								Structs                                        /
//=============================================================================/
typedef struct s_cmd
{
	char			**args;
	char			*input_file;
	char			*output_file;
	int				append;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_expand_state
{
	int	in_squote;
	int	in_dquote;
	int	i;
}	t_expand_state;

typedef struct s_expand_ctx
{
	t_expand_state	st;
	char			**envp;
	int				last_status;
	char			**res;
}	t_expand_ctx;

typedef struct s_expand_vars
{
	char	buf[16];
	int		j;
	char	var[128];
	int		len;
	char	*val;
}	t_expand_vars;

void	init_expand_vars(t_expand_vars *vars);
void	expand_token_handle(char c, t_expand_ctx *ctx);
void	expand_token_handle_dollar(const char *token, t_expand_ctx *ctx);
int		is_quote_to_skip(char c, int in_dquote, int in_squote);


//=============================================================================/
//								Builtins                                       /
//=============================================================================/

int		builtin_cd(char **args);
int		builtin_echo(char **args);
int		builtin_env(char **args);
int		builtin_exit(char **args);
int		builtin_export(char **args);
int		builtin_pwd(char **args);
int		builtin_unset(char **args);

//=============================================================================/
//								Executor                                       /
//=============================================================================/

int		exec_cmd(t_cmd *cmd, char **envp);

//=============================================================================/
//								Utils                                          /
//=============================================================================/

void	print_banner(void);
int		panic(char *error_msg);
int		fork_wrapper(void);


typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_REDIRECT_APPEND,
	TOKEN_REDIRECT_HEREDOC,
	TOKEN_EOF
} t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char		*value;
	struct s_token	*next;
} t_token;

typedef struct s_ast
{
	t_token_type	type;
	char		*value;
	struct s_ast	*left;
	struct s_ast	*right;
} t_ast;

t_token *lexer(const char *input);
void free_token_list(t_token *head);
t_ast *ast_new_node(t_token_type type, char *value);
void free_ast(t_ast *node);
void print_tokens(t_token *tok);
void print_ast(t_ast *node, int depth);

#endif
