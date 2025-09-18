#include "../../inc/parser.h"
#include "../../libraries/libft.h"
#include "../../libraries/ft_printf.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

static int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

int	builtin_cd(char **args)
{
	char	*path;
	char	*old_pwd;
	char	cwd[1024];

	if (args[2])
	{
		ft_printf("cd: too many arguments\n");
		return (1);
	}
	if (!args[1] || ft_strcmp(args[1], "~") == 0)
	{
		path = getenv("HOME");
		if (!path)
		{
			ft_printf("cd: HOME not set\n");
			return (1);
		}
	}
	else if (ft_strcmp(args[1], "-") == 0)
	{
		path = getenv("OLDPWD");
		if (!path)
		{
			ft_printf("cd: OLDPWD not set\n");
			return (1);
		}
	}
	else
		path = args[1];
	old_pwd = getcwd(cwd, sizeof(cwd));
	if (chdir(path) != 0)
	{
		perror("cd");
		return (1);
	}
	if (old_pwd)
		setenv("OLDPWD", old_pwd, 1);
	if (getcwd(cwd, sizeof(cwd)))
		setenv("PWD", cwd, 1);
	return (0);
}
