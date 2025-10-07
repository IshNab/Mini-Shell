/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 17:53:39 by maborges          #+#    #+#             */
/*   Updated: 2025/10/07 16:20:23 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static char	**env_to_array(t_env *env)
{
	char	**env_array;
	t_env	*current;
	int		i;
	int		count;
	char	*tmp;

	count = ft_envsize(env);
	env_array = safe_malloc(sizeof(char *) * (count + 1));
	current = env;
	i = 0;
	tmp = NULL;
	while (current)
	{
		tmp = ft_strjoin(current->key, "=");
		env_array[i] = ft_strjoin(tmp, current->value);
		free(tmp);
		current = current->next;
		i++;
	}
	env_array[i] = NULL;
	return (env_array);
}

static char	*join_path(char *str1, char *str2)
{
	char	*result_str;
	int		i;
	int		j;

	result_str = (char *)malloc(ft_strlen(str1) + ft_strlen(str2) + 2);
	i = -1;
	j = -1;
	while (str1[++i])
	{
		result_str[i] = str1[i];
	}
	result_str[i++] = '/';
	while (str2[++j])
		result_str[i++] = str2[j];
	result_str[i] = '\0';
	return (result_str);
}

static char	*find_cmd_path(char *cmd, t_env *env)
{
	int		i;
	t_env	*current;
	char	*path_name;
	char	**split_path;
	char	*full_path;
	char	*ret;

	i = 0;
	if (!cmd)
		return (NULL);
	path_name = NULL;
	current = env;
	ret = NULL;
	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	while (current)
	{
		if (ft_strcmp("PATH", current->key) == 0)
		{
			path_name = current->value;
			break ;
		}
		current = current->next;
	}
	if (!path_name)
		return (NULL);
	split_path = ft_split(path_name, ':');
	if (!split_path)
		return (NULL);
	i = -1;
	while (split_path[++i])
	{
		full_path = join_path(split_path[i], cmd);
		if (access(full_path, X_OK) == 0)
		{
			ret = full_path; // this saves the join_path malloc'd str
			break ;
		}
		free(full_path);
	}
	i = -1;
	while (split_path[++i])
		free(split_path[i]);
	free(split_path);
	return (ret);
}

static void	run_external_cmd(t_command *cmd, t_mshell *shell)
{
	char	*path;
	char	**env_array;
	int		i;

	if (!cmd || !cmd->base.args || !cmd->base.args[0])
		exit(127);
	path = find_cmd_path(cmd->base.args[0], shell->env);
	i = 0;
	if (!path)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->base.args[0], 2);
		ft_putstr_fd("\n", 2);
		exit(127);
	}
	env_array = env_to_array(shell->env);
	if (execve(path, cmd->base.args, env_array) == -1)
	{
		perror("minishell");
		free(path);
		while (env_array[i])
			free(env_array[i++]);
		free(env_array);
		//shell->exit_status = 126;
		exit(126);
	}
}

static int	try_builtin(t_command *cmd, t_mshell *shell)
{
	if (!cmd->base.args || !cmd->base.args[0])
		return (0);
	if (!ft_strcmp(cmd->base.args[0], "cd"))
		shell->exit_status = builtin_cd(cmd->base.args, shell);
	else if (!ft_strcmp(cmd->base.args[0], "pwd"))
		shell->exit_status = builtin_pwd(cmd->base.args);
	else if (!ft_strcmp(cmd->base.args[0], "echo"))
		shell->exit_status = builtin_echo(cmd->base.args);
	else if (!ft_strcmp(cmd->base.args[0], "env"))
		shell->exit_status = builtin_env(cmd->base.args, shell);
	else if (!ft_strcmp(cmd->base.args[0], "exit"))
		shell->exit_status = builtin_exit(cmd->base.args, shell);
	else if (!ft_strcmp(cmd->base.args[0], "export"))
		shell->exit_status = builtin_export(cmd->base.args, shell);
	else if (!ft_strcmp(cmd->base.args[0], "unset"))
		shell->exit_status = builtin_unset(cmd->base.args, shell);
	else
		return (0);
	return (1);
}

void	execute_simple_command(t_command *cmd, t_mshell *shell)
{
	int		status;
	pid_t	child_pid;
	int		fd;
	int		flags;

	if (!cmd || !cmd->base.args || !cmd->base.args[0])
		return ;
	if (try_builtin(cmd, shell))
		return ;
	child_pid = fork();
	if (child_pid == 0)
	{
		if (cmd->input_file)
		{
			fd = open(cmd->input_file, O_RDONLY);
			if (fd == -1)
			{
				perror("minishell");
				exit(1);
			}
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		if (cmd->output_file)
		{
			flags = O_WRONLY | O_CREAT;
			if (cmd->is_append)
				flags |= O_APPEND;
			else
				flags |= O_TRUNC;
			fd = open(cmd->output_file, flags, 0644);
			if (fd == -1)
			{
				perror("minishell");
				exit(1);
			}
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		run_external_cmd(cmd, shell);
		exit(127);
	}
	if (child_pid == -1)
	{
		perror("minishell: fork");
		shell->exit_status = 1;
		return ;
	}
	waitpid(child_pid, &status, 0);
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		shell->exit_status = 128 + WTERMSIG(status);
}

void	execute_ast(t_ast *ast, t_mshell *shell)
{
	t_command	*cmd;

	if (!ast)
		return ;
	if (ast->type == NODE_PIPE)
	{
		execute_pipe(ast, shell);
	}
	else if (ast->type == NODE_CMD)
	{
		cmd = (t_command *)ast;
		execute_simple_command(cmd, shell);
	}
}
