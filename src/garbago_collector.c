/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbago_collector.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sruff <sruff@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 17:29:50 by sruff             #+#    #+#             */
/*   Updated: 2025/07/05 18:22:21 by sruff            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

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