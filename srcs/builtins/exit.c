#include "../../inc/parser.h"
#include "../../libraries/libft.h"
#include "../../libraries/ft_printf.h"
#include <stdlib.h>
#include <stdio.h>

static int	is_numeric(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	builtin_exit(char **args)
{
	int	status;

	status = 0;
	if (args[1])
	{
		if (!is_numeric(args[1]))
		{
			ft_printf("exit: %s: numeric argument required\n", args[1]);
			exit(2);
		}
		status = ft_atoi(args[1]);
		if (args[2])
		{
			ft_printf("exit: too many arguments\n");
			return (1);
		}
	}
	ft_printf("exit\n");
	exit(status);
	return (0);
}
