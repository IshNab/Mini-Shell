/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 17:23:46 by maborges          #+#    #+#             */
/*   Updated: 2024/11/25 12:59:42 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* static void	ft_test(unsigned int i, char *str)
{
	*str = ft_toupper(*str);
} */

void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	int	i;
	int	j;

	if (!s || !f)
		return ;
	i = 0;
	j = ft_strlen(s);
	while (i < j)
	{
		(*f)(i, &s[i]);
		i++;
	}
}

/* int	main(void)
{
	char	str[] = "Oi gente linda!";

	ft_striteri(str, ft_test);
	printf("%s", str);
} */
