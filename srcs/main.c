/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 14:09:11 by maborges          #+#    #+#             */
/*   Updated: 2025/09/03 16:43:02 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

//REPL
// READ -> EVALUATE->PRINT->EXECUTE->LOOP

char	*cell_read_line(void)
{
	char *buf;
	size_t	buf_size;

	buf = NULL;

	if (getline(&buf, &buf_size, stdin) == -1)
	{
		buf = NULL;
		if (feof(stdin))
			printf(RED"[EOF]"RST);
		else
			printf(RED"Getline failed"RST);
		p("%s\n", buf);
	}

	return buf;
}

int	main(void)
{
	char	*line;
	char	cwd[BUFSIZ];
	char	*last_dir;

	line = NULL;
	print_banner();
	using_history();
	getcwd(cwd, sizeof(cwd));
	last_dir = strrchr(cwd, '/');
	while (1)
	{
		//printf("%s", last_dir);
		//line = readline("$");
		//printf("%s\n", line);
		//if (!line)
			//break ;
		//add_history(line);
		//free(line);
		line = cell_read_line();
		p("%s\n", line);
		pause();
	}
}
