/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 14:05:22 by maborges          #+#    #+#             */
/*   Updated: 2025/11/03 19:17:49 by maborges         ###   ########.fr       */
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

typedef struct s_ast
{
	t_node_type		type;
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
	struct s_ast	*left; //pipes: left cmd
	struct s_ast	*right; //pipes: right cmd
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

t_ast				*parser(char *input, char **envp, t_mshell *shell);
int					validate_syntax(t_token *tokens);
t_ast				*build_ast(t_token *tokens);
t_command			*create_command_node(t_token *tokens);
t_ast				*create_pipe_node(t_ast *left, t_ast *right);
void				free_ast(t_ast *node);

// Tokenizer
t_token				*ms_tokenize(const char *input);
void				free_token_list(t_token *head);
void				print_tokens(t_token *tok);

void				remove_quote_tokens(t_token **tokens);
//Variable Expansion
void				expand_vars(t_token *tokens, t_mshell *shell);
char				*expand_word(char *word, t_env *env, int exit_status, pid_t shell_pid);

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

//void				free_command(t_command *cmd);
//t_command			*mockup_parse(char *input, char **envp, int last_status);
//t_command			*create_mockup_command(char *input_line);
void				execute_ast(t_ast *ast, t_mshell *mshell);
void				execute_pipe(t_ast *pipe_node, t_mshell *shell);
void				run_external_cmd(t_command *cmd, t_mshell *shell);
int					try_builtin(t_command *cmd, t_mshell *shell);
int					is_parent_builtin(char *cmd_name);

//redirections
int					handle_input_redir(t_command *cmd, int *saved_stdin);
int					handle_output_redir(t_command *cmd, int *saved_stdout);
int					setup_redirections(t_command *cmd, int *saved_stdin, int *saved_stdout);
void				restore_redirections(int saved_stdin, int saved_stdout);

//=============================================================================/
//								Builtins                                       /
//=============================================================================/

int					builtin_cd(char **args, t_mshell *shell);
int					builtin_echo(char **args);
int					builtin_env(char **args, t_mshell *shell);
int					builtin_exit(char **args, t_mshell *shell);
int					builtin_export(char **arg, t_mshell *shell);
int					builtin_pwd(char **args);
int					builtin_unset(char **args, t_mshell *shell);

//=============================================================================/
//								Utils                                          /
//=============================================================================/

int					ft_envsize(t_env *env);
void				*safe_malloc(size_t size);
int					fork_wrapper(t_mshell *shell);
void				print_banner(void);

int					panic(char *error_msg);
int					error_msg(char *msg, int exit_code, t_mshell *shell);

char				*str_append(char *s1, const char *s2);
char				*get_env_from_list(t_env *env, const char *key);
void				unset_env_var(t_mshell *shell, const char *key);
void				sort_vars_list(t_env *env);
int					is_valid_identifier(char *s);
int					error_msg_export(char *id);

void				free_ast(t_ast *node);

//=============================================================================/
//								Signal Handling                                /
//=============================================================================/

extern volatile sig_atomic_t g_signal_received;

// Signal handling functions
void				signal_handler(int sig);
void				setup_signals(void);
void				setup_interactive_signals(void);
void				setup_non_interactive_signals(void);
void				default_child_signals(void);
void				heredoc_signal_handler(int sig);
void				setup_heredoc_signals(void);
int					create_heredoc_file(char *delimiter);

// Global signal state
extern volatile sig_atomic_t			g_signal_received;

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
