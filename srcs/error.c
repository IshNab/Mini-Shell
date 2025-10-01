/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 12:31:37 by maborges          #+#    #+#             */
/*   Updated: 2025/10/01 21:15:57 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

//panic is for safe_malloc errors and critical initialization

int	panic(char *error_msg)
{
	ft_putstr_fd("Error: ", 2);
	ft_putstr_fd(error_msg, 2);
	exit(1);
}

int	error_msg(char *msg, int exit_code, t_mshell *shell)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(msg, 2);
	ft_putstr_fd("\n", 2);
	if (shell)
		shell->exit_status = exit_code;
	return (exit_code);
}

