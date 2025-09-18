#include "../../inc/parser.h"
#include "../../libraries/libft.h"
#include "../../libraries/ft_printf.h"
#include <stdio.h>
#include <unistd.h>

int	builtin_pwd(char **args)
{
	char	cwd[1024];

	(void)args;
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		ft_printf("%s\n", cwd);
		return (0);
	}
	else
	{
		perror("pwd");
		return (1);
	}
}
