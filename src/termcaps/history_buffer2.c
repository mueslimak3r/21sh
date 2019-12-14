/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_buffer2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/13 18:47:31 by calamber          #+#    #+#             */
/*   Updated: 2019/12/13 18:49:34 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

int			check_empty(t_tbuff *buff)
{
	int	i;

	i = 0;
	if (!buff || !buff->buff_str)
		return (1);
	while (buff->buff_str[i] && ft_isspace(buff->buff_str[i]))
		i++;
	if (buff->buff_str[i])
		return (0);
	return (1);
}

void		tbuff_set_default(t_tbuff *buff, int hd)
{
	buff->next = NULL;
	buff->temp = false;
	buff->original = NULL;
	buff->heredocs = hd ? true : false;
}

int			tbuff_choose(t_tbuff **buff, int hd)
{
	t_tbuff *new;

	new = NULL;
	if (!buff || !*buff || !(*buff)->buff_str || !*((*buff)->buff_str)
												|| check_empty(*buff))
	{
		while ((*buff)->next)
			*buff = (*buff)->next;
		return (0);
	}
	tbuff_new(&new);
	if (new)
	{
		new = ft_memcpy(new, *buff, sizeof(*new));
		new->buff_str = ft_strdup((*buff)->buff_str);
		tbuff_set_default(new, hd);
		while ((*buff)->next)
			*buff = (*buff)->next;
		new->prev = *buff;
		if (*buff)
			(*buff)->next = new;
		*buff = new;
		return (1);
	}
	return (0);
}

void		tbuff_rm_edits_helper(t_tbuff **buff)
{
	t_tbuff	*tmp;

	tmp = *buff;
	if (tmp->next)
	{
		*buff = tmp->next;
		tmp->next->prev = tmp->prev;
		if (tmp->prev)
			tmp->prev->next = tmp->next;
	}
	else if (tmp->prev)
	{
		*buff = tmp->prev;
		tmp->prev->next = tmp->next;
		if (tmp->next)
			tmp->next->prev = tmp->prev;
	}
	else
		*buff = NULL;
}

void		tbuff_rm_edits(t_tbuff **buff)
{
	t_tbuff *tmp;

	if (!buff || !*buff)
		return ;
	tmp = *buff;
	if (tmp->original)
	{
		if (tmp->next)
			tmp->next->prev = tmp->original;
		if (tmp->prev)
			tmp->prev->next = tmp->original;
		tmp->original->next = tmp->next;
		tmp->original->prev = tmp->prev;
		*buff = tmp->original;
		tbuff_rm_node(tmp);
		return ;
	}
	else
	{
		tbuff_rm_edits_helper(buff);
		tbuff_rm_node(tmp);
	}
}
