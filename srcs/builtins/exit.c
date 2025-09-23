/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inabakka <inabakka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 14:32:23 by maborges          #+#    #+#             */
/*   Updated: 2025/09/23 17:34:20 by inabakka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static int ft_is_numeric(const char *s)
{
    if (!s || !*s) return 0;
    if (*s == '+' || *s == '-') s++;
    while (*s)
    {
        if (!ft_isdigit((unsigned char)*s)) return 0;
        s++;
    }
    return 1;
}

int builtin_exit(char **args)
{
    int argc = 0;
    while (args[argc]) argc++;

    fprintf(stderr, "exit\n"); //function not allowed

    if (argc == 1)
        exit(0);

    if (!ft_is_numeric(args[1]))
    {
        fprintf(stderr, "exit: %s: numeric argument required\n", args[1]);
        exit(2);
    }

    if (argc > 2)
    {
        fprintf(stderr, "exit: too many arguments\n");
        return 1; // do not exit, as bash does
    }

    // Convert to unsigned char semantics (0-255)
    long long val = strtoll(args[1], NULL, 10); //function not allowed
    exit((unsigned char)val);
}
