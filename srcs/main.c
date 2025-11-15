/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 16:46:27 by maborges          #+#    #+#             */
/*   Updated: 2025/11/14 17:41:40 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_mshell	shell;

	if (argc > 1)
		return (error_msg("Minishell does not accept arguments", 1, NULL));
	(void)argv;
	print_banner();
	using_history();
	init_shell(&shell, envp);
	setup_interactive_signals();
	repl_loop(&shell);
	cleanup_shell(&shell);
	return (shell.exit_status);
}
