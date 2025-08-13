/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbago_collector.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 17:29:50 by sruff             #+#    #+#             */
/*   Updated: 2025/08/09 17:39:12 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

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

t_gc	*get_gc_addr(void)
{
	static t_gc	gc = {0};

	return (&gc);
}

t_gc_node	*gc_add_node(void *address)
{
	t_gc_node	*new_node;

	new_node = malloc(sizeof(t_gc_node));
	if (!new_node)
		return (NULL);
	new_node->ptr = address;
	new_node->next = NULL;
	return (new_node);
}

void	gc_add_start(void *ptr)
{
	t_gc_node	*new_node;
	t_gc		*gc;

	gc = get_gc_addr();
	new_node = gc_add_node(ptr);
	if (!new_node)
	{
		ft_printf("gc malloc failed in file %s at line %d\n", __FILE__,
			__LINE__);
		return ;
	}
	if (!gc->first)
	{
		gc->first = new_node;
		gc->last = new_node;
	}
	else
	{
		new_node->next = gc->first;
		gc->first = new_node;
	}
	gc->len += 1;
}

void	*gc_malloc(size_t size)
{
	void	*ptr;

	ptr = malloc(size);
	if (!ptr)
		return (NULL);
	gc_add_start(ptr);
	return (ptr);
}

t_gc	*gc_init(void)
{
	t_gc	*gc;

	gc = get_gc_addr();
	gc->first = NULL;
	gc->last = NULL;
	gc->len = 0;
	return (gc);
}

void	gc_free_all(void)
{
	t_gc *gc = get_gc_addr();
	t_gc_node *current = gc->first;
	t_gc_node *temp;

	while (current)
	{
		temp = current->next;
		free(current->ptr);
		free(current);
		current = temp;
	}
	gc->first = NULL;
	gc->last = NULL;
	gc->len = 0;
}
