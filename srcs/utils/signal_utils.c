/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/24 14:30:10 by inabakka          #+#    #+#             */
/*   Updated: 2025/11/09 18:55:52 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// Single global to store last received signal number (subject requirement)
// volatile because variable can change anytime, compiler read variable from
//memory line every time
// sig_atomic can be read/ written in one operation
// works safely for signal handlers

volatile sig_atomic_t	g_signal_received = 0;

void	signal_handler(int sig)
{
	//actual handling is done in main loop/parent; set a flag
	g_signal_received = sig;
	if (sig == SIGINT)
	{
		//write(STDOUT_FILENO, "\n", 1);
		ioctl(STDIN_FILENO, TIOCSTI, "\n");
	}
}

void	setup_interactive_signals(void)
{
	struct sigaction sa_int;	//sigint interupts signals
	struct sigaction sa_quit;	//sigquit ignores/quits signals

	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0; // do not use SA_RESTART so readline can be interrupted
	sa_int.sa_handler = signal_handler;
	sigaction(SIGINT, &sa_int, NULL);

	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sa_quit.sa_handler = SIG_IGN; // ctrl-\ does nothing in interactive mode
	sigaction(SIGQUIT, &sa_quit, NULL);
}

void	setup_non_interactive_signals(void)
{
	struct sigaction sa_int;
	struct sigaction sa_quit;

	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	sa_int.sa_handler = signal_handler; // parent tracks SIGINT while launching
	sigaction(SIGINT, &sa_int, NULL);

	// In parent, ignore SIGQUIT during command launch; child restores defaults
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sa_quit.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa_quit, NULL);
}

void	default_child_signals(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = SIG_DFL;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
	//should we also restore SIGPIPE?
}

// legacy compatibility
void	setup_signals(void)
{
	setup_interactive_signals();
}

//child process: set up heredoc signals and read input
// read line until delimiter is found or EOF is reached
// write line to temporary file

//parent process: wait for child and handle signals
// check if child was terminated by signal
// if interrupted, cleans up temp file
