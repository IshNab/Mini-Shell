/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inabakka <inabakka@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 13:17:15 by inabakka          #+#    #+#             */
/*   Updated: 2024/12/18 20:32:34 by inabakka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H
# include <unistd.h>
# include <stdarg.h>

int			ft_print_percent(void);
void		ft_putchar_length(char character, int *length);
int			ft_putchar(char c);
int			ft_putstr(char *str);
int			ft_print_unsigned(unsigned int u);
int			ft_puthex_lower(unsigned long nb);
int			ft_puthex_upper(unsigned int nb);
int			ft_print_ptr(unsigned long nb);
int			ft_printf(const char *format, ...);
int			ft_putnbr(int nb);

#endif
