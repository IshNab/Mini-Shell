/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 14:27:31 by maborges          #+#    #+#             */
/*   Updated: 2025/11/12 14:30:44 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static int	cd_error(char *path, t_mshell *shell)
{
	ft_putstr_fd("minishell: cd: ", 2);
	ft_putstr_fd(path, 2);
	ft_putstr_fd(": ", 2);
	perror("");
	shell->exit_status = 1;
	return (1);
}

static char	*get_target_path(char **args, t_mshell *shell)
{
	char	*home;
	char	*oldpwd;

	if (!args[1] || ft_strcmp(args[1], "~") == 0
		|| ft_strcmp(args[1], "--") == 0)
	{
		home = getenv("HOME");
		if (!home)
			return (error_msg("minishell: cd: HOME not set", 1, shell), NULL);
		return (home);
	}
	if (ft_strcmp(args[1], "-") == 0)
	{
		oldpwd = getenv("OLDPWD");
		if (!oldpwd)
			return (error_msg("cd: OLDPWD not set", 1, shell), NULL);
		ft_putendl_fd(oldpwd, 1);
		return (oldpwd);
	}
	return (args[1]);
}

static void	update_pwd_vars(t_mshell *shell, char *old_pwd)
{
	char	*new_pwd;

	new_pwd = getcwd(NULL, 0);
	if (new_pwd)
	{
		set_env_var(shell, "PWD", new_pwd);
		free(new_pwd);
	}
	if (old_pwd)
	{
		set_env_var(shell, "OLDPWD", old_pwd);
		free(old_pwd);
	}
}

int	builtin_cd(char **args, t_mshell *shell)
{
	char	*target;
	char	*old_pwd;

	if (args[1] && args[2])
		return (error_msg("minishell: cd: too many arguments", 1, shell));
	target = get_target_path(args, shell);
	if (!target)
		return (1);
	old_pwd = getcwd(NULL, 0);
	if (chdir(target) != 0)
	{
		free(old_pwd);
		return (cd_error(target, shell));
	}
	update_pwd_vars(shell, old_pwd);
	shell->exit_status = 0;
	return (0);
}

/* //DEBUG cd()
	int i = 0;
	printf("DEBUG cd args:\n");
	while (args[i])
	{
		printf("  args[%d] = '%s'\n", i, args[i]);
		i++;
	}
	printf("  args[%d] = NULL\n", i); */
