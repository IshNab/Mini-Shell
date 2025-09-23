/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inabakka <inabakka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 14:09:11 by maborges          #+#    #+#             */
/*   Updated: 2025/09/23 17:14:12 by inabakka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"
#include "../inc/parser.h"

//REPL
// READ -> EVALUATE->PRINT->EXECUTE->LOOP

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	char	cwd[BUFSIZ];
	char	*last_dir;

	(void)argc;
	(void)argv;
	line = NULL;
	print_banner();
	setup_signals();
	using_history();
	while (1)
	{
		if (!line)
		{
			ft_printf("exit\n");
			break ;
			add_history(line);
			parse_command(line, envp);
			free(line);
		}
	}
}
