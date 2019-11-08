/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_buffer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/25 03:27:26 by calamber          #+#    #+#             */
/*   Updated: 2019/11/07 21:53:11 by calamber         ###   ########.fr       */
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

int     move_cursor(int amt)
{
    if (g_term.conf.cursor[0] + amt > (ft_strlen(g_term.line_in) + 2))
        return (0);
    if (g_term.conf.cursor[0] + amt > g_term.conf.termsize[0])
    {
        g_term.conf.cursor[0] = amt;
        g_term.conf.cursor[1]++;
        g_term.conf.curlines++;
    }
    else if (g_term.conf.cursor[0] + amt < 0)
    {
        g_term.conf.cursor[0] = g_term.conf.termsize[0] - 1;
        g_term.conf.cursor[1]--;
        g_term.conf.curlines--;
    }
    else if (g_term.conf.cursor[0] + amt <= ft_strlen(g_term.line_in))
        g_term.conf.cursor[0] += amt;
    return (1);
}

void		tbuff_move_cursor(t_tbuff *buff, unsigned long code, char *str)
{
	if (code == LEFT || code == RIGHT)
		move_cursor(code == LEFT ? -1 : 1);
	if ((code == LEFT && buff->rope_buff_cursor - 1 < 0) ||
		(code == RIGHT && buff->rope_buff_cursor + 1 > LEAF_SIZE) ||
		code == UP || code == DOWN)
	{
		if (buff->rope_buff_pos > 0)
		{
			buff->rope = rope_insert(buff->rope, buff->rope_buff, buff->cursor + 1);
			ft_memset(buff->rope_buff, 0, LEAF_SIZE);
			buff->rope_buff_pos = 0;
			buff->rope_buff_cursor = 0;
		}
		if (code == LEFT || code == RIGHT)
			buff->cursor += (code == LEFT) ? -1 : 1;
		else
		{
			if (code == UP)
			{
				//ft_printf_fd(STDERR_FILENO, "going up\n");
				if (g_term.curr_buff && g_term.curr_buff->next)
				{
					g_term.curr_buff = g_term.curr_buff->next;
					reprint_buffer(g_term.curr_buff, 1);
				}
			}
			if (code == DOWN)
			{
				//ft_printf_fd(STDERR_FILENO, "going down\n");
				if (g_term.curr_buff && g_term.curr_buff->prev)
				{
					g_term.curr_buff = g_term.curr_buff->prev;
					reprint_buffer(g_term.curr_buff, 1);
				}
			}
		}
		return ;
	}
	if (code == LEFT || code == RIGHT)
	{
		buff->rope_buff_cursor += (code == LEFT) ? -1 : 1;
		//ft_printf_fd(STDERR_FILENO, "%s", str);
	}
}

void		tbuff_insert_text(t_tbuff *buff, char *rope_buff, char *input, int size)
{
	//int size = ft_strlen(input);
	//ft_printf_fd(STDERR_FILENO, "inserting %s at bpos: %d\n", input, buff->rope_buff_pos);
	if (buff->rope_buff_cursor < buff->rope_buff_pos)
		ft_memmove(rope_buff + buff->rope_buff_cursor, rope_buff + buff->rope_buff_cursor, size - buff->rope_buff_cursor);
	buff->rope_buff_pos += size;
	ft_memcpy(rope_buff + buff->rope_buff_cursor, input, size);
	buff->rope_buff_cursor += size;
}

void		tbuff_rope_add(t_tbuff *buff, char *rope_buff, char *input)
{
				//int pos = g_term.conf.termsize[0] * g_term.conf.cursor[1] + g_term.conf.cursor[0] - 2;
			//g_term.curr_buff->rope = rope_insert(g_term.curr_buff->rope, buf, pos + 1);
	int size = ft_strlen(input);
	if (buff->rope_buff_pos + size == LEAF_SIZE)
	{
		tbuff_insert_text(buff, buff->rope_buff, input, size);
		buff->rope = rope_insert(buff->rope, rope_buff, buff->cursor + 1);
		ft_memset(rope_buff, 0, LEAF_SIZE + 1);
		buff->rope_buff_pos = 0;
		buff->rope_buff_cursor = 0;
	}
	else if (buff->rope_buff_pos + size < LEAF_SIZE)
		tbuff_insert_text(buff, buff->rope_buff, input, size);
	else if (buff->rope_buff_pos + size > LEAF_SIZE)
	{
		ft_printf_fd(STDERR_FILENO, "overflow: pos %d size: %d\n", buff->rope_buff_pos, size);
		int leftover = size;
		tbuff_insert_text(buff, buff->rope_buff, input, LEAF_SIZE - buff->rope_buff_pos);
		buff->rope = rope_insert(buff->rope, rope_buff, buff->cursor + 1);
		ft_memset(rope_buff, 0, LEAF_SIZE + 1);
		buff->rope_buff_pos = 0;
		buff->rope_buff_cursor = 0;
		leftover -= LEAF_SIZE - buff->rope_buff_pos;
		while (leftover)
		{
			ft_printf_fd(STDERR_FILENO, "loop leftover: %d\n", leftover);
			tbuff_insert_text(buff, buff->rope_buff, input, (leftover >= LEAF_SIZE) ? LEAF_SIZE : leftover);
			buff->rope = rope_insert(buff->rope, rope_buff, buff->cursor + 1);
			ft_memset(rope_buff, 0, LEAF_SIZE + 1);
			buff->rope_buff_pos = 0;
			buff->rope_buff_cursor = 0;
			leftover -= (leftover >= LEAF_SIZE) ? LEAF_SIZE : leftover;
		}
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
