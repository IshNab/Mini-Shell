/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 14:05:22 by maborges          #+#    #+#             */
/*   Updated: 2025/09/24 13:21:37 by maborges         ###   ########.fr       */
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

//===========//
//Token types//
//===========//
typedef enum	e_token_type
{
	TOKEN_WORD, //regular words/cmds/arguments
	TOKEN_PIPE, //||
	TOKEN_REDIRECT_IN, // <
	TOKEN_REDIRECT_OUT, // >
	TOKEN_APPEND, // >>
	TOKEN_HEREDOC, // <<
	TOKEN_EOF, // End of input, end of file
	TOKEN_ERROR // invalid token
}	t_token_type;

//Individual token structures
typedef struct s_token
{
	t_token_type	type;
	char			*value; //string value
	int				quote_type; // 0=none, 1=single, 2=double
	struct s_token	*next; //next token  in list
	struct s_token	*prev; // Previous
}	t_token;
//Redirecion types
typedef enum e_redirect_type
{
	REDIRECT_INPUT, // < file
	REDIRECT_OUTPUT, // > file
	REDIRECT_APPEND, // >> file
	REDIRECT_HEREDOC // << delimiter
}	t_redirect_type;

//Single redirection structure
typedef struct s_redirect
{
	t_redirect_type		type;
	char				*file;
	int					fd;
	struct s_redirect	*next;
}	t_redirect;

//===========//
//AST STRUCTS//
//===========//
//AST node types
// Parser creates, Executor traverses
typedef enum e_ast_type
{
	AST_COMMAND,
	AST_PIPE
}	t_ast_type;

// Main AST node structure.
// Used for main communication between parser and executor
// Parser creates/populates and executor Executes
typedef struct s_ast
{
	t_ast_type		type; //what cmd it is? simple cmd, pipe?
	//below the ones used when type = AST_COMMAND
	char			**argv;
	int				argc;
	t_redirect		*redirections; //TODO
	//tree structure, used for pipes
	struct s_ast	*left;
	struct s_ast	*right;
	pid_t			pid;
	int				exit_status;
}	t_ast;

//==================//
//ENVIRONMENT SYSTEM//
//==================//
// Environment variable structure
// shared resource for both teams

typedef struct s_env
{
	char			*key; // variable name
	char			*value; // variable value
	int				exported; //1 if exported, 0 if local only
	struct s_env	*next; //Next environment variable
}	t_env;

//=================//
//EXECUTION CONTEXT//
//=================//
//Pipeline execution context
typedef struct s_pipeline
{
	int				**pipes; // array of pipe fd
	pid_t			*pids; //array of pids
	int				cmd_count; //nbr of cmd in pipeline
	int				current_cmd; //current cmd [i] being exec
}	t_pipeline;

//Builtin command function pointer
typedef int	(*t_builtin_func)(char **argv, t_env **env);
//builtin cmd struct
typedef struct s_builtin
{
	char			*name; //cmd name
	t_builtin_func	func; //function pointer
}	t_builtin;

//==================//
//SHELL STATE STRUCT//
//==================//
//Signal handling states
typedef enum e_signal_state
{
	SIGNAL_INTERACTIVE, //waiting for input
	SIGNAL_EXECUTING, // executing command
	SIGNAL_HEREDOC // reading heredoc
}	t_signal_state;

//This is the master structure that holds all shell state
typedef struct s_msh
{
	//environment
	t_env			*env; //environment variables
	char			**envp; // Environment array for execve
	//Execution state
	int				exit_status; //last cmd exit status
	int				must_exit; //flag for when u must exit shell
	pid_t			shell_pid; //shell pid
	//signa handling
	t_signal_state	signal_state; //current signal handling state
	int				interrupted; //flag for signal interruption
	//IO
	char			*line; //current cmd/input line
	int				stdin_fd; // OG stdin
	int				stdout_fd; //OG stdout
	int				stderr_fd; //OG stderr
	//Parse state
	t_token			*tokens; //current token list
	t_ast			*ast; //current AST
	//execution state
	t_pipeline		*pipeline; //current pipeline context
}	t_msh;

/* typedef struct s_cmd
{
	char			**args;
	char			*input_file;
	char			*output_file;
	int				append; //0 no, 1 yes
	struct s_cmd	*next;
}	t_cmd; */


//=============================================================================/
//								Parser                                         /
//=============================================================================/

int					parse_command(char *input, char **envp);

//=============================================================================/
//								Builtins                                       /
//=============================================================================/

int					builtin_cd(char **args);
int					builtin_echo(char **args);
int					builtin_env(char **args);
int					builtin_exit(char **args);
int					builtin_export(char **args);
int					builtin_pwd(char **args);
int					builtin_unset(char **args);

//=============================================================================/
//								Executor                                       /
//=============================================================================/

int					exec_cmd(t_cmd *cmd, char **envp); //TODO substitute the t_cmd

//=============================================================================/
//								Utils                                          /
//=============================================================================/

void				print_banner(void);
int					panic(char *error_msg);
int					fork_wrapper(void);

#endif
