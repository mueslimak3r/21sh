/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_buffer.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/25 03:27:26 by calamber          #+#    #+#             */
/*   Updated: 2019/12/11 03:58:40 by calamber         ###   ########.fr       */
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
	//while (tmp && tmp->next)
	//	tmp = tmp->next;
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

int			tbuff_choose(t_tbuff **buff, int hd)
{
	t_tbuff *new;

	new = NULL;
	if (!buff || !*buff || !(*buff)->buff_str || !*((*buff)->buff_str) || check_empty(*buff))
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
		new->temp = false;
		new->original = NULL;
		new->heredocs = hd ? true : false;
		while ((*buff)->next)
			*buff = (*buff)->next;
		new->next = NULL;
		new->prev = *buff;
		if (*buff)
			(*buff)->next = new;
		*buff = new;
		return (1);
	}
	return (0);
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
		tbuff_rm_node(tmp);
	}
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

void		tbuff_free_hd(t_tbuff **buff)
{
	t_tbuff	*list;

	if (!buff || !*buff)
		return ;
	list = *buff;	
	while (list)
	{
		if (list->heredocs)
		{
			tbuff_rm_edits(&list);
		}
		else if (list->prev)
			list = list->prev;
		else
			break ;
	}
	while (list->next)
		list = list->next;
	*buff = list;
}
