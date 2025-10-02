/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inabakka <inabakka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 14:05:22 by maborges          #+#    #+#             */
/*   Updated: 2025/09/25 15:32:05 by maborges         ###   ########.fr       */
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
# include <string.h> //strerror
# include <sys/wait.h>
# include <sys/stat.h> //check the need
# include <fcntl.h> // check the need
# include <errno.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../libft/libft.h"
# include "./executor.h"

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

// Token types for lexer
typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_REDIRECT_APPEND,
	TOKEN_REDIRECT_HEREDOC
}	t_token_type;

// Token structure for lexer
typedef struct s_token
{
	t_token_type		type;
	char				*value;
	struct s_token		*next;
}	t_token;

//Redirecion types
typedef enum e_node_type
{
	NODE_CMD,
	NODE_PIPE,
	NODE_REDIR
}	t_node_type;

typedef struct s_expand_state
{
	int	in_squote;
	int	in_dquote;
	int	i;
}	t_expand_state;

typedef struct s_ast
{
	t_node_type		type;
	struct s_ast	*left; //pipes: left cmd
	struct s_ast	*right; //pipes: right cmd
	int				exit_status;
	char			**args; //command arguments
}	t_ast;
typedef enum e_redir_type
{
	REDIR_INPUT, // <
	REDIR_OUTPUT, // >
	REDIR_APPEND, // >>
	REDIR_HEREDOC // <<
}	t_redir_type;
typedef struct s_redir
{
	t_ast			base;
	char			*filename;
	t_node_type		type;
}	t_redir;

typedef struct s_command
{
	t_ast			base;
	char			**args;
	char			*input_file; //< input
	char			*output_file; // > or >> output
	int				is_append; //flag for no=0 yes=1
	char			*heredoc_delimiter; //<< delimiter
}	t_command;

//pipe node
typedef struct s_pipeline
{
	t_ast			base;
}	t_pipeline;

typedef struct s_env
{
	char			*key; // variable name
	char			*value; // variable value
	struct s_env	*next; //Next environment variable
}	t_env;

typedef struct s_mshell
{
	t_env			*env; //environment variables
	t_ast			*ast; //current AST
	int				cmd_count;
	int				exit_status; //last cmd exit status
	int				must_exit; //flag for when u must exit shell
	pid_t			shell_pid; //shell pid
}	t_mshell;

//=============================================================================/
//								Lexer & Parser                                 /
//=============================================================================/

// Lexer functions
t_token				*lexer(const char *input);
t_token				*new_token(t_token_type type, char *value);
void				free_token_list(t_token *head);
void				print_tokens(t_token *tok);
int					is_operator(const char *s);

// Parser functions
int					parse_command(char *input, char **envp);
int					validate_syntax(t_token *tokens);
t_ast				*build_simple_ast(t_token *tokens);
void				parse_and_exec_ast(t_token *tokens);

// AST functions
t_ast				*ast_new_node(t_node_type type, char *value);
void				free_ast(t_ast *node);
void				print_ast(t_ast *node, int depth);

//=============================================================================/
//								Utils                                          /
//=============================================================================/

void				print_banner(void);
int					panic(char *error_msg);
int					fork_wrapper(void);
void				*safe_malloc(size_t size);

#endif
