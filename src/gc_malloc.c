/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gc_malloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stefan <stefan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 17:29:50 by sruff             #+#    #+#             */
/*   Updated: 2025/08/14 16:32:15 by stefan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

t_gc	*get_gc_addr(void)
{
	static t_gc	gc = {0};

	return (&gc);
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
	t_gc		*gc;
	t_gc_node	*current;
	t_gc_node	*temp;

	gc = get_gc_addr();
	current = gc->first;
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
