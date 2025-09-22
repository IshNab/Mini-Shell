/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inabakka <inabakka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 16:35:22 by maborges          #+#    #+#             */
/*   Updated: 2025/09/22 15:58:02 by inabakka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	print_banner(void)
{
	ft_printf(
		GREEN"███╗   ███╗██╗███╗   ██╗██╗███████╗██╗  ██╗███████╗██╗     ██╗\n"
		"████╗ ████║██║████╗  ██║██║██╔════╝██║  ██║██╔════╝██║     ██║\n"
		"██╔████╔██║██║██╔██╗ ██║██║███████╗███████║█████╗  ██║     ██║\n"
		"██║╚██╔╝██║██║██║╚██╗██║██║╚════██║██╔══██║██╔══╝  ██║     ██║\n"
		"██║ ╚═╝ ██║██║██║ ╚████║██║███████║██║  ██║███████╗███████╗███████╗\n"
		"╚═╝     ╚═╝╚═╝╚═╝  ╚═══╝╚═╝╚══════╝╚═╝╚═╝╚══════╝╚══════╝╚══════╝\n\n"RESET);
}

void	signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		ft_printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	setup_signals(void)
{
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, SIG_IGN);
}

char *ft_strtok(char *str, const char *delim)
{
    static char *save_ptr = NULL;
    char *token_start;
    int i;

    if (str)
        save_ptr = str;
    if (!save_ptr || *save_ptr == '\0')
        return NULL;

    // 跳过前导分隔符
    while (*save_ptr && strchr(delim, *save_ptr))
        save_ptr++;
    if (*save_ptr == '\0')
        return NULL;

    token_start = save_ptr;
    // 查找下一个分隔符
    while (*save_ptr && !strchr(delim, *save_ptr))
        save_ptr++;
    if (*save_ptr)
    {
        *save_ptr = '\0';
        save_ptr++;
    }
    else
    {
        save_ptr = NULL;
    }
    return token_start;
}
