/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 20:10:40 by alkozma           #+#    #+#             */
/*   Updated: 2019/12/09 11:37:11 by alkozma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

static int		handle_up_down(int code, t_tbuff **buff)
{
	int		i;
	int		len;

	i = g_term.conf.curlines;
	if (code == DOWN && (!*buff || !(*buff)->next))
		return (0);
	if (*buff && (
				((*buff)->next && code == DOWN)
				|| ((*buff)->prev && code == UP)))
	{
		tputs(tgetstr("cr", NULL), 0, ft_charput);
		while (i-- > 1)
			tputs(tgetstr("sr", NULL), 0, ft_charput);
		tputs(tgetstr("cd", NULL), 0, ft_charput);
		(*buff) = code == UP
			? (*buff)->prev : (*buff)->next;
		len = (*buff)->len;
		g_term.conf.cursor[0] = g_term.conf.prompt_size;
		g_term.conf.cursor[1] = 0;
		g_term.conf.curlines = 1;
		reprint_buffer(*buff, 0);
		move_cursor(len, 0, *buff);
	}
	return (1);
}

static int		handle_arrows(int code, t_tbuff **buff)
{
	int	pos;

	pos = calc_pos();
	if (code == UP || code == DOWN)
		return (handle_up_down(code, buff));
	if ((code == LEFT && pos - 1 >= 0)
	 || 
		(code == RIGHT && pos + 1 <= (*buff)->len))
	{
		//if ((code == RIGHT && g_term.conf.cursor[0] + 1 < g_term.conf.termsize[0]) ||
		//(code == LEFT && g_term.conf.cursor[0] - 1 >= 0))
		//	tputs(tgetstr(code == LEFT ? "le" : "nd", NULL), 0, ft_charput);
		move_cursor(code == LEFT ? -1 : 1, 1, *buff);
	}
	return (0);
}

int		zero_cursor(void)
{
	int	size;

	size = g_term.conf.prompt_size;
	g_term.conf.termsize[0] = g_window_size.ws_col;
	g_term.conf.termsize[1] = g_window_size.ws_row;
	g_term.conf.cursor[1] = size != 0 && g_term.conf.termsize[0] != 0 ? (size / g_term.conf.termsize[0]) : 0;
	g_term.conf.cursor[0] = size != 0  && g_term.conf.termsize[0] != 0 ? (size % g_term.conf.termsize[0]) : 0;
	g_term.conf.curlines = g_term.conf.cursor[1] + 1;
	return (1);
}

static int		handle_enter(t_tbuff *buff)
{
	if (buff && buff->buff_str)
		while (g_term.conf.curlines > 1)
			g_term.conf.curlines--;
	tputs(tgetstr("cr", NULL), 0, ft_charput);
	tputs(tgetstr("sf", NULL), 0, ft_charput);
	zero_cursor();
	return (1);
}

int				handle_controls(unsigned long code, t_tbuff **buff)
{
	int ret;
	int	cursor_pos;

	ret = 0;
	cursor_pos = calc_pos();
	if (code == DELETE)
	{
		if (cursor_pos > 0)
		{
			if (buff && *buff && !(*buff)->temp)
				tbuff_replicate(buff);
			t_buff_line_rm(*buff, --cursor_pos, 1);
			move_cursor(-1, 1, *buff);
			reprint_buffer(*buff, cursor_pos);
		}
	}
	else if (code == ENTER)
		handle_enter(*buff);
	else if (code == UP || code == DOWN ||
		code == LEFT || code == RIGHT)
		handle_arrows(code, buff);
	else
		ret -= 1;
	ret += 1;
	return (ret);
}
