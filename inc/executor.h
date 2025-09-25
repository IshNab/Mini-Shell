/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 17:58:36 by maborges          #+#    #+#             */
/*   Updated: 2025/09/25 12:28:00 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "./minishell.h"

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

int					execute_ast(t_ast *ast, t_mshell *mshell);


# endif
