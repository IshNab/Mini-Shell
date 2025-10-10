/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 14:30:10 by inabakka          #+#    #+#             */
/*   Updated: 2025/10/04 21:18:06 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// Global variable to track if we're in interactive mode
int g_interactive_mode = 1;

void	signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		if (g_interactive_mode)
		{
			// Interactive mode: display new prompt
			ft_printf("\n");
			rl_on_new_line();
			rl_replace_line("", 0);
			rl_redisplay();
		}
		else
		{
			// Non-interactive mode: terminate current command
			exit(130); // Standard exit code for SIGINT
		}
	}
}

void	setup_interactive_signals(void)
{
	g_interactive_mode = 1;
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	setup_non_interactive_signals(void)
{
	g_interactive_mode = 0;
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	restore_default_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

// Legacy function for backward compatibility
void	setup_signals(void)
{
	setup_interactive_signals();
}
