/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_buffer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/25 03:27:26 by calamber          #+#    #+#             */
/*   Updated: 2019/11/08 16:27:20 by calamber         ###   ########.fr       */
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
	new->rope_size = 0;//ft_strlen(s);
	*buff = new;
}

void		tbuff_move_cursor(t_tbuff *buff, unsigned long code, char *str)
{
	if ((code == LEFT && buff->rope_buff_cursor - 1 < 0) ||
		(code == RIGHT && buff->rope_buff_cursor + 1 > LEAF_SIZE) ||
		code == UP || code == DOWN)
	{
		if (buff->rope_buff_pos >= 0 && buff->rope_buff[0])
		{
			buff->rope = rope_insert(buff->rope, buff->rope_buff, buff->cursor + 1);
			ft_memset(buff->rope_buff, 0, LEAF_SIZE);
			buff->rope_buff_pos = 0;
			buff->rope_buff_cursor = 0;
		}
		if ((code == LEFT || code == RIGHT) && buff->rope_buff_pos == 0)
			buff->cursor += (code == LEFT) ? -1 : 1;
		else
		{
			if (code == UP)
			{
				if (g_term.curr_buff && g_term.curr_buff->next)
				{
					g_term.curr_buff = g_term.curr_buff->next;
					g_term.curr_buff->cursor = sum_length(g_term.curr_buff->rope);
					reprint_buffer(g_term.curr_buff);
				}
			}
			if (code == DOWN)
			{
				if (g_term.curr_buff && g_term.curr_buff->prev)
				{
					g_term.curr_buff = g_term.curr_buff->prev;
					g_term.curr_buff->cursor = sum_length(g_term.curr_buff->rope);
					reprint_buffer(g_term.curr_buff);
				}
			}
		}
		return ;
	}
	if (code == LEFT || code == RIGHT)
	{
		buff->rope_buff_cursor += (code == LEFT) ? -1 : 1;
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
