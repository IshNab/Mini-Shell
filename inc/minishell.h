/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 14:05:22 by maborges          #+#    #+#             */
/*   Updated: 2025/10/06 20:01:01 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#define DEBUG 0

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
# include <ctype.h> // need to take this out before release
# include <errno.h>
# include <signal.h>
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

// Token types for lexer
typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
	TOKEN_SQUOTE,
	TOKEN_DQUOTE,
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

/* typedef struct s_expand_state // Ishta gonna check what it is and if we need
{
	int	in_squote;
	int	in_dquote;
	int	i;
}	t_expand_state; */


typedef struct s_ast
{
	t_node_type		type;
	struct s_ast	*left; //pipes: left cmd
	struct s_ast	*right; //pipes: right cmd
	int				exit_status; //should this be here? maybe only on the shell->exit_status
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

//UNCOMMENT BEFORE RELEASE
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
//								Parser                                         /
//=============================================================================/

int		parse_command(char *input, char **envp);
char	*ms_remove_quotes(const char *token);
int		is_quote(char c);
char	*str_append(char *s, char c);
char	*expand_status(int last_status);
char	*get_env_value(const char *var, char **envp);
t_token	*ms_tokenize(const char *input);
char	*ms_expand_token(const char *token, char **envp, int last_status);
void	expand_token_loop(const char *token, char **res, char **envp,
			int last_status);
void	expand_token_handle(char c, t_expand_state *ctx);
void	expand_token_handle_dollar(const char *token, t_expand_state *ctx,
			char **envp, int last_status, char **res);
char	*expand_variable(const char *token, int *i, char **envp, int last_status);
void	expand_and_append(char **res, char *tmp);

//=============================================================================/
//								Executor                                       /
//=============================================================================/

/* //MOCKUP PARSER (DELETE THIS BEFORE RELEASE)
// Simple mockup structure for testing
typedef struct s_command {
    char **args;        // Array of arguments
    char *input_file;   // Input redirection
    char *output_file;  // Output redirection
    int append;         // Append mode for output
    struct s_command *next; // For pipe chains
} t_command; */

void				free_command(t_command *cmd);
//t_command			*mockup_parse(char *input, char **envp, int last_status);
//t_command			*create_mockup_command(char *input_line);
void				execute_command(t_command *cmd, t_mshell *shell);



int					execute_ast(t_ast *ast, t_mshell *mshell);

//=============================================================================/
//								Builtins                                       /
//=============================================================================/

int					builtin_cd(char **args, t_mshell *shell);
int					builtin_echo(char **args);
int					builtin_env(char **args);
int					builtin_exit(char **args);
int					builtin_export(char **args);
int					builtin_pwd(char **args);
int					builtin_unset(char **args);

//=============================================================================/
//								Lexer & Parser                                 /
//=============================================================================/

// Tokenizer helspers
char				*ft_strndup(const char *s, size_t n);
void	handle_word(const char *input, int *i, t_token **head,
	t_token **tail);
void				free_token_list(t_token *head);
void				print_tokens(t_token *tok);

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

int					ft_envsize(t_env *env);
void				*safe_malloc(size_t size);
int					fork_wrapper(void);
void				print_banner(void);

int					panic(char *error_msg);
int					error_msg(char *msg, int exit_code, t_mshell *shell);


//=============================================================================/
//								Debug                                          /
//=============================================================================/

// Debug functions
void debug_print_env(t_env *env);
void debug_print_command(t_command *cmd);
void debug_print_shell(t_mshell *shell);
void debug_checkpoint(const char *function, int line, const char *message);

#define DEBUG_CHECKPOINT(msg) debug_checkpoint(__FUNCTION__, __LINE__, msg)

#endif
