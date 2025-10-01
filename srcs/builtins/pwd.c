/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 14:33:35 by maborges          #+#    #+#             */
/*   Updated: 2025/10/01 15:49:53 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int builtin_pwd(char **args)
{
	char	cwd[1024];

	write(1, "begin of builtin pwd\n", 23);
	(void)args;
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		printf("%s\n", cwd);
	else
		perror("pwd");
	write(1, "end of builtin pwd\n", 21);
	return (0);
}
