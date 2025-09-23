/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_function.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 12:31:37 by maborges          #+#    #+#             */
/*   Updated: 2025/09/23 15:28:00 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	panic(char *error_msg)
{
	ft_putstr_fd("Error\n", 2);
	ft_putstr_fd(error_msg, 2);
	exit(1);
}
