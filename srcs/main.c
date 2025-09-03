/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 14:09:11 by maborges          #+#    #+#             */
/*   Updated: 2025/09/03 14:21:25 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	main(int ac, char **av)
{
	int	status;

	(void)ac;
	if (fork() == 0) //init child proccess
	{
		execvp(av[1], av + 1);
	}
	wait(&status);
	return (EXIT_SUCCESS); //check if EXIT_SUCCESS can be used in this  project
}
