/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_buffer.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/25 03:27:26 by calamber          #+#    #+#             */
/*   Updated: 2019/12/13 18:47:12 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

void		tbuff_new(t_tbuff **buff)
{
	t_tbuff *new;
	t_tbuff	*tmp;

	if (!(new = ft_memalloc(sizeof(t_tbuff))))
		return ;
	tmp = *buff;
	if (*buff)
	{
		new->prev = tmp;
		tmp->next = new;
		new->next = NULL;
	}
	else
	{
		new->next = NULL;
		new->prev = NULL;
	}
	new->original = NULL;
	new->temp = true;
	new->len = 0;
	new->buff_str = NULL;
	*buff = new;
}

void		tbuff_replicate(t_tbuff **buff)
{
	t_tbuff *new;

	new = NULL;
	if (!buff || !*buff || (*buff)->temp)
		return ;
	tbuff_new(&new);
	if (new)
	{
		new = ft_memcpy(new, *buff, sizeof(*new));
		new->buff_str = ft_strdup((*buff)->buff_str);
		new->temp = true;
		new->original = *buff;
		if ((*buff)->prev)
			(*buff)->prev->next = new;
		if ((*buff)->next)
			(*buff)->next->prev = new;
		*buff = new;
	}
}

void		tbuff_rm_node(t_tbuff *node)
{
	if (node->buff_str)
		ft_strdel(&(node->buff_str));
	node->next = NULL;
	node->prev = NULL;
	node->original = NULL;
	free(node);
}

void		tbuff_cleanup(t_tbuff **buff)
{
	t_tbuff	*list;

	if (!buff || !*buff)
		return ;
	list = *buff;
	while (list)
	{
		if (list->temp)
			tbuff_rm_edits(&list);
		else if (list->prev)
			list = list->prev;
		else
			break ;
	}
	while (list && list->next)
		list = list->next;
	*buff = list;
}

void		tbuff_free(t_tbuff **buff)
{
	t_tbuff *tmp;

	if (!buff || !*buff)
		return ;
	while (*buff)
	{
		tmp = *buff;
		*buff = (*buff)->prev;
		if (tmp->buff_str)
			ft_strdel(&(tmp->buff_str));
		free(tmp);
	}
	*buff = NULL;
}
