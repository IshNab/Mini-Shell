/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 14:28:17 by maborges          #+#    #+#             */
/*   Updated: 2025/10/08 16:00:08 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	builtin_echo(char **args)
{
	int	i;
	int	no_newline;
	int	j;

	i = 1;
	no_newline = 0;
	// Support multiple -n flags like echo -n -n
	while (args[i] && args[i][0] == '-' )
	{
		j = 1;
		if (args[i][j] == '\0')
			break ; // just "-"
		while (args[i][j] == 'n')
			j++;
		if (args[i][j] != '\0')
			break ; // not purely -n...n
		no_newline = 1;
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
