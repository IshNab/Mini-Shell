/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 19:15:12 by maborges          #+#    #+#             */
/*   Updated: 2025/11/03 19:18:22 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	is_parent_builtin(char *cmd_name)
{
	if (!ft_strcmp(cmd_name, "cd"))
		return (1);
	if (!ft_strcmp(cmd_name, "export"))
		return (1);
	if (!ft_strcmp(cmd_name, "unset"))
		return (1);
	if (!ft_strcmp(cmd_name, "exit"))
		return (1);
	return (0);
}

int	try_builtin(t_command *cmd, t_mshell *shell)
{
	if (!cmd->args || !cmd->args[0])
		return (0);
	if (!ft_strcmp(cmd->args[0], "cd"))
		shell->exit_status = builtin_cd(cmd->args, shell);
	else if (!ft_strcmp(cmd->args[0], "pwd"))
		shell->exit_status = builtin_pwd(cmd->args);
	else if (!ft_strcmp(cmd->args[0], "echo"))
		shell->exit_status = builtin_echo(cmd->args);
	else if (!ft_strcmp(cmd->args[0], "env"))
		shell->exit_status = builtin_env(cmd->args, shell);
	else if (!ft_strcmp(cmd->args[0], "exit"))
		shell->exit_status = builtin_exit(cmd->args, shell);
	else if (!ft_strcmp(cmd->args[0], "export"))
		shell->exit_status = builtin_export(cmd->args, shell);
	else if (!ft_strcmp(cmd->args[0], "unset"))
		shell->exit_status = builtin_unset(cmd->args, shell);
	else
		return (0);
	return (1);
}
