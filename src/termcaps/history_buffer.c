/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_buffer.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/25 03:27:26 by calamber          #+#    #+#             */
/*   Updated: 2019/11/22 21:54:21 by alkozma          ###   ########.fr       */
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
	while (tmp && tmp->next)
		tmp = tmp->next;
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
	ft_printf("prev: %s\n", tmp ? tmp->buff_str : "NULL");
	new->len = 0;
	new->buff_str = NULL;
	*buff = new;
}

void		tbuff_free(t_tbuff **buff)
{
	t_tbuff *tmp;

	if (!buff || !*buff)
		return ;
	while (*buff)
	{
		tmp = *buff;
		*buff = (*buff)->next;
		if (tmp == *buff)
			*buff = NULL;
		if (tmp->buff_str)
			ft_strdel(&(tmp->buff_str));
		free(tmp);
	}
	*buff = NULL;
}
