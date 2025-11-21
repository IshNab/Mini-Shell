/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 14:05:22 by maborges          #+#    #+#             */
/*   Updated: 2025/11/20 12:53:54 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define DEBUG 0

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
# include <sys/ioctl.h>
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
	t_token_type	type;
	char			*value;
	struct s_token	*next;
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
	REDIR_INPUT,
	REDIR_OUTPUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}	t_redir_type;
typedef struct s_redir
{
	t_ast				base;
	char				*filename;
	t_node_type			type;
}	t_redir;

//UNCOMMENT BEFORE RELEASE
typedef struct s_command
{
	t_ast				base;
	char				**args;
	char				*input_file; //< input
	char				*output_file; // > or >> output
	int					is_append; //flag for no=0 yes=1
	char				*heredoc_delimiter; //<< delimiter
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
	int				exported;
	struct s_env	*next; //Next environment variable
}	t_env;

typedef struct s_mshell
{
	t_env			*env;
	t_ast			*ast;
	int				cmd_count;
	int				exit_status;
	int				must_exit;
	pid_t			shell_pid;
}	t_mshell;

//=============================================================================/
//								Lexer & Parser                                 /
//=============================================================================/

t_ast				*parser(char *input, t_mshell *shell);
int					validate_syntax(t_token *tokens);
t_ast				*build_ast(t_token *tokens);
t_command			*create_command_node(t_token *tokens);
int					process_word_token(t_command *cmd, t_token *token, int *i);
int					process_tokens(t_command *cmd, t_token *tokens, int *i);
t_ast				*create_pipe_node(t_ast *left, t_ast *right);
void				free_ast(t_ast *node);

// Tokenizer
t_token				*ms_tokenize(char *input);
void				free_token_list(t_token *head);
void				list_token_append(t_token *new, t_token **head,
						t_token **tail);
int					handle_redirections(t_token *new, char *input, int *i);

void				remove_quote_tokens(t_token **tokens);
//Variable Expansion
void				expand_vars(t_token *tokens, t_mshell *shell);
char				*exp_word(char *word, t_mshell *shell);

//=============================================================================/
//								Executor                                       /
//=============================================================================/

void				execute_ast(t_ast *ast, t_mshell *mshell);
void				execute_pipe(t_ast *pipe_node, t_mshell *shell);
void				run_external_cmd(t_command *cmd, t_mshell *shell);
int					try_builtin(t_command *cmd, t_mshell *shell);
int					is_parent_builtin(char *cmd_name);

//redirections
int					handle_input_redir(t_command *cmd, int *saved_stdin);
int					handle_output_redir(t_command *cmd, int *saved_stdout);
int					setup_redirections(t_command *cmd, int *saved_stdin,
						int *saved_stdout);
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
char				**env_to_array(t_env *env);
char				*get_env_from_list(t_env *env, const char *key);
void				unset_env_var(t_mshell *shell, const char *key);
void				set_env_var(t_mshell *shell, char *key, char *value);
void				sort_vars_list(t_env *env);
int					is_valid_identifier(char *s);
int					error_msg_export(char *id);

void				free_ast(t_ast *node);
void				free_env_array(char **env_array);
void				cleanup_shell(t_mshell *shell);

void				repl_loop(t_mshell *shell);
void				process_command(t_mshell *shell, char *line);
void				signal_on_main(t_mshell *shell, char **line);
void				init_shell(t_mshell *shell, char **envp);
t_env				*init_env(char **envp);
//=============================================================================/
//								Signal Handling                                /
//=============================================================================/

extern volatile sig_atomic_t	g_signal_received;

// Signal handling functions
void				signal_handler(int sig);
void				setup_signals(void);
void				setup_interactive_signals(void);
void				setup_non_interactive_signals(void);
void				default_child_signals(void);
void				heredoc_signal_handler(int sig);
void				setup_heredoc_signals(void);
int					create_heredoc_file(char *delimiter);
void				wait_child_process(pid_t child_pid, t_mshell *shell);

//=============================================================================/
//								Debug                                          /
//=============================================================================/

// Debug functions
void				debug_print_env(t_env *env);
void				debug_print_command(t_command *cmd);
void				debug_print_shell(t_mshell *shell);

#endif
