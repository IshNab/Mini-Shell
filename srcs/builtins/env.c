/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 14:28:47 by maborges          #+#    #+#             */
/*   Updated: 2025/09/19 14:29:18 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

extern char **environ; // can we use this?

int builtin_env(char **args)
{
    (void)args; // env ignores extra args in many shells; 42 subject might require error if args exist
    for (char **e = environ; e && *e; ++e)
        puts(*e);
    return 0;
}
