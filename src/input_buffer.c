/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_buffer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/25 03:27:26 by calamber          #+#    #+#             */
/*   Updated: 2019/10/31 22:32:21 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

void		tbuff_new(t_tbuff **buff)
{
	t_tbuff *new;

	/*
	if (!s || !*s)
		return ;
	while (*s == 27)
		s++;
	if (!*s)
		return ;
	*/
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
	new->rope = NULL;
	//ft_printf_fd(STDERR_FILENO, "len of buff input: %d\n", (int)ft_strlen(s));
	//createRopeStructure(&new->rope, NULL, s, 0, ft_strlen(s) - 1);
	new->size = 0;//ft_strlen(s);
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
		if (tmp->rope)
			rope_free(tmp->rope);
		free(tmp);
	}
	*buff = NULL;
}
