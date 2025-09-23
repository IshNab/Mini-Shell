/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inabakka <inabakka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 14:28:17 by maborges          #+#    #+#             */
/*   Updated: 2025/09/23 17:34:00 by inabakka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int builtin_echo(char **args)
{
    int i = 1;
    int no_newline = 0;

    // Support multiple -n flags like echo -n -n
    while (args[i] && args[i][0] == '-' )
    {
        int j = 1;
        if (args[i][j] == '\0') break; // just "-"
        while (args[i][j] == 'n') j++;
        if (args[i][j] != '\0') break; // not purely -n...n
        no_newline = 1;
        i++;
    }

    for (; args[i]; ++i)
    {
        fputs(args[i], stdout);
        if (args[i + 1]) fputc(' ', stdout);
    }
    if (!no_newline) fputc('\n', stdout);
    return 0;
}
