/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_buffer.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/25 03:27:26 by calamber          #+#    #+#             */
/*   Updated: 2019/11/18 19:44:30 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

void		tbuff_new(t_tbuff **buff)
{
	t_tbuff *new;

	if (!(new = ft_memalloc(sizeof(t_tbuff))))
		return ;
	if (*buff)
	{
		new->prev = (*buff)->prev;
		if ((*buff)->prev)
			(*buff)->prev->next = new;
		(*buff)->prev = new;
		new->next = *buff;
	}
	else
	{
		new->next = NULL;
		new->prev = NULL;
	}
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
