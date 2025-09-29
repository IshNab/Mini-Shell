/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maborges <maborges@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 16:39:40 by maborges          #+#    #+#             */
/*   Updated: 2024/12/02 15:28:29 by maborges         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_wordcount(const char *s, char c);
static char	*ft_words(const char *s, int *start_word, char c);
static void	ft_free(char **array, size_t size);

char	**ft_split(char const *s, char c)
{
	char		**tmparray;
	size_t		i;
	int			start_word;
	size_t		words;

	if (!s)
		return (NULL);
	tmparray = (char **)malloc((ft_wordcount(s, c) + 1) * sizeof(char *));
	if (!tmparray)
		return (NULL);
	words = ft_wordcount(s, c);
	i = 0;
	start_word = 0;
	while (i < words)
	{
		tmparray[i] = ft_words(s, &start_word, c);
		if (!tmparray[i])
		{
			ft_free(tmparray, i);
			return (NULL);
		}
		i++;
	}
	tmparray[i] = NULL;
	return (tmparray);
}

static int	ft_wordcount(const char *s, char c)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (s[i])
	{
		if (s[i] != c && (i == 0 || s[i - 1] == c))
			j++;
		i++;
	}
	return (j);
}

static char	*ft_words(const char *s, int *start_word, char c)
{
	char	*word;
	int		i;
	int		len;

	i = 0;
	len = 0;
	while (s[*start_word] && s[*start_word] == c)
		(*start_word)++;
	while (s[*start_word + len] && s[*start_word + len] != c)
		len++;
	word = malloc((len + 1) * sizeof(char));
	if (!word)
		return (NULL);
	while (i < len)
	{
		word[i] = s[*start_word + i];
		i++;
	}
	word[i] = '\0';
	*start_word += len;
	return (word);
}

static void	ft_free(char **array, size_t size)
{
	size_t	i;

	i = 0;
	while (i < size)
	{
		free(array[i]);
		i++;
	}
	free(array);
}
