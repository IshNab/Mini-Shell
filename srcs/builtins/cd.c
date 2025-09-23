/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 14:27:31 by maborges          #+#    #+#             */
/*   Updated: 2025/09/23 16:21:13 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static int update_pwd_vars(const char *oldpwd)
{
	char *newpwd = getcwd(NULL, 0);

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
	setenv("PWD", newpwd, 1);
	free(newpwd);
	return (0);
}

int	builtin_cd(char **args)
{
	int	argc;

	argc = 0;
	while (args[argc])
		argc++;
	if (argc > 2)
	{
		panic("cd: too many arguments\n");
		return (1);
	}
	const char *target = NULL;
	const char *home = getenv("HOME");
	printf("home folder name: %s\n", home);
	if (argc == 1 || (args[1] && (strcmp(args[1], "~") == 0 || ft_strcmp(args[1], "--") == 0)))
	{
		if (!home)
		{
			fprintf(stderr, "cd: HOME not set\n");
			return (1);
		}
		target = home;
	}
	else if (strcmp(args[1], "-") == 0)
	{
		const char *oldpwd = getenv("OLDPWD");
		if (!oldpwd)
		{
			fprintf(stderr, "cd: OLDPWD not set\n");
			return (1);
		}
		target = oldpwd;
	}
	else
		target = args[1];
	char *oldpwd = getcwd(NULL, 0);
	if (chdir(target) != 0)
	{
		perror("cd");
		free(oldpwd);
		return (1);
	}
	// If 'cd -', print the new directory
	if (args[1] && strcmp(args[1], "-") == 0)
	{
		char *pwd = getcwd(NULL, 0);
		if (pwd)
		{
			printf("%s\n", pwd);
			free(pwd);
		}
	}
	int rc = update_pwd_vars(oldpwd);
	free(oldpwd);
	return rc;
}
