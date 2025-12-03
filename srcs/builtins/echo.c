/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 14:28:17 by maborges          #+#    #+#             */
/*   Updated: 2025/11/12 13:23:59 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static int	n_flag_handler(char **args, int i, int *no_newline)
{
	int	j;

	j = 1;
	if (args[i][j] == '\0')
		return (0);
	while (args[i][j] == 'n')
		j++;
	if (args[i][j] != '\0')
		return (0);
	*no_newline = 1;
	return (1);
}

int	builtin_echo(char **args)
{
	int	i;
	int	no_newline;

	i = 1;
	no_newline = 0;
	while (args[i] && args[i][0] == '-' )
	{
		if (!n_flag_handler(args, i, &no_newline))
			break ;
		i++;
	}
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (!no_newline)
		printf("\n");
	return (0);
}
