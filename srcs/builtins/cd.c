/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 14:27:31 by maborges          #+#    #+#             */
/*   Updated: 2025/10/08 16:02:03 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static int	update_pwd_vars(const char *oldpwd)
{
	char	*newpwd;

	newpwd = getcwd(NULL, 0);
	if (!newpwd)
	{
		perror("cd: getcwd");
		// Best effort: still try to update OLDPWD if provided
		if (oldpwd)
			setenv("OLDPWD", oldpwd, 1);
		return (1);
	}
	if (oldpwd)
		setenv("OLDPWD", oldpwd, 1);
	setenv("PWD", newpwd, 1); //Illegal function
	free(newpwd);
	return (0);
}

int	builtin_cd(char **args, t_mshell *shell)
{
	const char	*target;
	const char	*home;
	char		*oldpwd;
	char		*pwd;
	int			rc;

	target = NULL;
	home = getenv("HOME");
	if (args[1] && args[2])
		return (error_msg("cd: too many arguments\n", 1, NULL));
	else if (!args[1] || (args[1] && (ft_strcmp(args[1], "~") == 0 ||
				ft_strcmp(args[1], "--") == 0)))
	{
		if (!home)
			return (error_msg("cd: HOME not set\n", 1, shell));
		target = home;
	}
	else if (ft_strcmp(args[1], "-") == 0)
	{
		oldpwd = getenv("OLDPWD");
		if (!oldpwd)
			return (error_msg("cd: OLDPWD not set\n", 1, shell));
		target = oldpwd;
	}
	else
		target = args[1];
	oldpwd = getcwd(NULL, 0);
	if (chdir(target) != 0)
	{
		perror("cd");
		return (free(oldpwd), 1);
	}
	if (args[1] && strcmp(args[1], "-") == 0)
	{
		pwd = getcwd(NULL, 0);
		if (pwd)
		{
			printf("%s\n", pwd);
			free(pwd);
		}
	}
	rc = update_pwd_vars(oldpwd);
	free(oldpwd);
	return (rc);
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
