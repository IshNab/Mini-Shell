/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 14:32:23 by maborges          #+#    #+#             */
/*   Updated: 2025/11/20 12:05:29 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static int	ft_isspace(int c)
{
	return (c == ' ' || c == '\t' || c == '\n'
		|| c == '\v' || c == '\f' || c == '\r');
}

static int	is_valid_number(const char *str)
{
	int	i;

	i = 0;
	while (ft_isspace(str[i]))
		i++;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i] && !ft_isspace(str[i]))
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	builtin_exit(char **args, t_mshell *shell)
{
	int	argc;
	int	exit_code;

	argc = 0;
	while (args[argc])
		argc++;
	ft_putstr_fd("exit\n", 2);
	if (argc == 1)
		exit(shell->exit_status);
	if (!is_valid_number(args[1]))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(args[1], 2);
		ft_putendl_fd(": numeric argument required", 2);
		exit(2);
	}
	if (argc == 2)
		shell->exit_status = ft_atoi(args[1]);
	if (argc > 2)
	{
		error_msg("exit: too many arguments", 1, shell);
		return (1);
	}
	exit_code = ft_atoi(args[1]);
	exit((unsigned char)exit_code);
}
