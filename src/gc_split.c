/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 17:29:50 by sruff             #+#    #+#             */
/*   Updated: 2025/08/14 16:32:52 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

static int	get_wordlen(char const *s, char c)
{
	int	i;

	i = 0;
	while (s[i] && s[i] != c)
	{
		i++;
	}
	return (i);
}

static int	get_index(char const *s, char c, int index)
{
	int	i;

	i = index;
	while (s[i])
	{
		if (s[i] != c)
		{
			return (i);
		}
		i++;
	}
	return (i);
}

static int	word_counter(char const *s, char c)
{
	int	i;
	int	words;

	i = 0;
	words = 0;
	while (s[i])
	{
		if (s[i] != c && (s[i + 1] == c || !s[i + 1]))
		{
			words++;
		}
		i++;
	}
	return (words);
}

static char	*gc_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	char	*substr;

	if (!s)
		return (NULL);
	if (start >= ft_strlen(s))
		return (gc_strdup(""));
	if (len > ft_strlen(s) - start)
		len = ft_strlen(s) - start;
	substr = gc_malloc(len + 1);
	if (!substr)
		return (NULL);
	i = 0;
	while (i < len)
	{
		substr[i] = s[start + i];
		i++;
	}
	substr[i] = '\0';
	return (substr);
}

char	**gc_split(char const *s, char c)
{
	int		words;
	int		i;
	int		index;
	char	**ptrarray;
	int		wlen;

	i = 0;
	index = get_index(s, c, i);
	words = word_counter(s, c);
	ptrarray = gc_malloc((words + 1) * sizeof(char *));
	if (!ptrarray)
		return (0);
	ptrarray[words] = NULL;
	while (i < words)
	{
		wlen = get_wordlen(&s[index], c);
		ptrarray[i] = gc_substr(s, index, wlen);
		if (!ptrarray[i])
			return (NULL);
		index = get_index(s, c, index + wlen);
		i++;
	}
	return (ptrarray);
}
