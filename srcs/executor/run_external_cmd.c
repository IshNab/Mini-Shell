/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_external_cmd.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 19:12:06 by maborges          #+#    #+#             */
/*   Updated: 2025/11/10 15:57:40 by maborges         ###   ########.fr       */
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
	env_array = malloc(sizeof(char *) * (count + 1));
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

void	run_external_cmd(t_command *cmd, t_mshell *shell)
{
	char	*path;
	char	**env_array;
	int		i;

	if (!cmd || !cmd->args || !cmd->args[0])
		exit(127);
	env_array = env_to_array(shell->env);
	path = find_cmd_path(cmd->args[0], shell->env);
	i = 0;
	if (!path)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->args[0], 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		while (env_array[i])
			free(env_array[i++]);
		free(env_array);
		exit(127);
	}
	if (execve(path, cmd->args, env_array) == -1)
	{
		perror("minishell");
		free(path);
		while (env_array[i])
			free(env_array[i++]);
		free(env_array);
		exit(126);
	}
}
