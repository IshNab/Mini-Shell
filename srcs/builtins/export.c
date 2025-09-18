#include "../../inc/parser.h"
#include "../../libraries/libft.h"
#include "../../libraries/ft_printf.h"
#include <stdio.h>
#include <stdlib.h>

static int	is_valid_var_name(char *name)
{
	int	i;

	i = 0;
	if (!ft_isalpha(name[i]) && name[i] != '_')
		return (0);
	i++;
	while (name[i])
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	builtin_export(char **args)
{
	extern char	**environ;
	int			i;
	char		*key;
	char		*value;

	i = 0;
	if (!args[1])
	{
		while (environ[i])
		{
			ft_printf("declare -x %s\n", environ[i]);
			i++;
		}
		return (0);
	}
	key = args[1];
	value = ft_strchr(key, '=');
	if (!value)
	{
		ft_printf("export: invalid format, use KEY=VALUE\n");
		return (1);
	}
	*value = '\0';
	value++;
	if (!is_valid_var_name(key))
	{
		ft_printf("export: '%s': not a valid identifier\n", key);
		return (1);
	}
	if (setenv(key, value, 1) != 0)
	{
		perror("export");
		return (1);
	}
	return (0);
}
