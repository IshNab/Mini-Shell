/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 14:29:06 by maborges          #+#    #+#             */
/*   Updated: 2024/11/28 18:15:32 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/* static char	ft_test(unsigned int i, char str);
static char	uppercase(unsigned int i, char c); */

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char			*str;
	unsigned int	i;

	if (!s || !f)
		return (NULL);
	i = 0;
	str = malloc((ft_strlen(s) + 1) * sizeof(char));
	if (!str)
		return (NULL);
	while (i < ft_strlen(s))
	{
		str[i] = (*f)(i, s[i]);
		i++;
	}
	str[i] = '\0';
	return (str);
}

/* static char	ft_test(unsigned int i, char c)
{
	return (c);
}

static char	uppercase(unsigned int i, char c)
{
	return (ft_toupper(c));
}

int	main(void)
{
	char const	*s1 = "Oi gente linda!";
	char		*strmapistr;
	char		*strmapistr2;

	strmapistr = ft_strmapi(s1, ft_test);
	strmapistr2 = ft_strmapi(s1, uppercase);
	printf("test1: %s\n", strmapistr);
	printf("%s", strmapistr2);
	return (0);
} */
