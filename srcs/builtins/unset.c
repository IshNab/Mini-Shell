#include "../../inc/parser.h"
#include "../../libraries/libft.h"
#include "../../libraries/ft_printf.h"
#include <stdio.h>

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

int	builtin_unset(char **args)
{
	if (!args[1])
	{
		ft_printf("unset: missing argument\n");
		return (1);
	}
	if (!is_valid_var_name(args[1]))
	{
		ft_printf("unset: '%s': not a valid identifier\n", args[1]);
		return (1);
	}
	if (unsetenv(args[1]) != 0)
	{
		perror("unset");
		return (1);
	}
	return (0);
}
