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
	ft_memset(new->rope_buff, 0, LEAF_SIZE + 1);
	new->rope_buff_pos = 0;
	new->rope = NULL;
	//ft_printf_fd(STDERR_FILENO, "len of buff input: %d\n", (int)ft_strlen(s));
	//createRopeStructure(&new->rope, NULL, s, 0, ft_strlen(s) - 1);
	new->size = 0;//ft_strlen(s);
	*buff = new;
}

void		tbuff_rope_add(t_tbuff *buff, char *rope_buff, char *input)
{
				//int pos = g_term.conf.termsize[0] * g_term.conf.cursor[1] + g_term.conf.cursor[0] - 2;
			//g_term.curr_buff->rope = rope_insert(g_term.curr_buff->rope, buf, pos + 1);
	int size = ft_strlen(input);
	if (buff->rope_buff_pos + size > LEAF_SIZE)
	{
		ft_memcpy(rope_buff + buff->rope_buff_pos, input, size);
		buff->rope = rope_insert(buff->rope, rope_buff, buff->cursor);
	}
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
