/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 14:09:11 by maborges          #+#    #+#             */
/*   Updated: 2025/09/23 15:43:01 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

//REPL
// READ -> EVALUATE->PRINT->EXECUTE->LOOP

int	main(int argc, char **argv, char **envp)
{
	char	*line;

	(void)argc;
	(void)argv;
	line = NULL;
	print_banner();
	using_history();
	while (1)
	{
		line = readline("$");
		//printf("%s\n", line);
		if (!line)
			break ;
		add_history(line);
		parse_command(line, envp);
		free(line);
	}
}
