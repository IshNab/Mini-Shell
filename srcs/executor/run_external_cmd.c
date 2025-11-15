/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_external_cmd.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 19:12:06 by maborges          #+#    #+#             */
/*   Updated: 2025/11/14 18:47:42 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

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

static char	*search_in_paths(char **split_path, char *cmd)
{
	char	*full_path;
	int		i;
	int		j;

	i = -1;
	while (split_path[++i])
	{
		full_path = join_path(split_path[i], cmd);
		if (access(full_path, X_OK) == 0)
		{
			j = -1;
			while (split_path[++j])
				free(split_path[j]);
			free(split_path);
			return (full_path);
		}
		free(full_path);
	}
	i = -1;
	while (split_path[++i])
		free(split_path[i]);
	free(split_path);
	return (NULL);
}

static char	*find_cmd_path(char *cmd, t_env *env)
{
	t_env	*current;
	char	**split_path;
	char	*ret;

	if (!cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	current = env;
	while (current)
	{
		if (ft_strcmp("PATH", current->key) == 0)
		{
			split_path = ft_split(current->value, ':');
			if (!split_path)
				return (NULL);
			ret = search_in_paths(split_path, cmd);
			return (ret);
		}
		current = current->next;
	}
	return (NULL);
}

static void	handle_parent_wait(pid_t pid, t_mshell *shell)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		shell->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		shell->exit_status = 128 + WTERMSIG(status);
}

void	run_external_cmd(t_command *cmd, t_mshell *shell)
{
	char	*cmd_path;
	char	**env_array;
	pid_t	pid;

	cmd_path = find_cmd_path(cmd->args[0], shell->env);
	if (!cmd_path)
	{
		ft_putstr_fd(cmd->args[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		shell->exit_status = 127;
		return ;
	}
	env_array = env_to_array(shell->env);
	pid = fork();
	if (pid == 0)
	{
		execve(cmd_path, cmd->args, env_array);
		perror("execve");
		exit(127);
	}
	handle_parent_wait(pid, shell);
	free(cmd_path);
	free_env_array(env_array);
}
