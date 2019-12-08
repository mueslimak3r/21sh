/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 20:10:40 by alkozma           #+#    #+#             */
/*   Updated: 2019/12/07 20:32:57 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

static int		handle_up_down(int code)
{
	int		i;
	int		len;

	i = g_term.conf.curlines;
	if (code == DOWN && (!g_term.curr_buff || !g_term.curr_buff->next))
		return (0);
	if (g_term.curr_buff && (
				(g_term.curr_buff->next && code == DOWN)
				|| (g_term.curr_buff->prev && code == UP)))
	{
		tputs(tgetstr("cr", NULL), 0, ft_charput);
		while (i-- > 1)
			tputs(tgetstr("sr", NULL), 0, ft_charput);
		tputs(tgetstr("cd", NULL), 0, ft_charput);
		g_term.curr_buff = code == UP
			? g_term.curr_buff->prev : g_term.curr_buff->next;
		len = g_term.curr_buff->len;
		g_term.conf.cursor[0] = g_term.conf.prompt_size;
		g_term.conf.cursor[1] = 0;
		g_term.conf.curlines = 1;
		reprint_buffer(g_term.curr_buff);
		move_cursor(len, 0);
	}
	return (1);
}

static int		handle_arrows(int code)
{
	int	pos;

	pos = calc_termsize();
	if (code == UP || code == DOWN)
		return (handle_up_down(code));
	if ((code == LEFT && pos - 1 >= 0)
	 || 
		(code == RIGHT && pos + 1 <= g_term.curr_buff->len))
	{
		if ((code == RIGHT && g_term.conf.cursor[0] + 1 < g_term.conf.termsize[0]) ||
		(code == LEFT && g_term.conf.cursor[0] - 1 >= 0))
			tputs(tgetstr(code == LEFT ? "le" : "nd", NULL), 0, ft_charput);
		move_cursor(code == LEFT ? -1 : 1, 1);
	}
	return (0);
}

int		zero_cursor(void)
{
	int	size;

	size = g_term.conf.prompt_size;
	g_term.conf.cursor[1] = size / g_term.conf.termsize[0];
	g_term.conf.cursor[0] = size % g_term.conf.termsize[0];
	g_term.conf.curlines = g_term.conf.cursor[1] + 1;
	return (1);
}

static int		handle_enter(void)
{
	if (g_term.curr_buff && g_term.curr_buff->buff_str)
		while (g_term.conf.curlines > 1)
			g_term.conf.curlines--;
	tputs(tgetstr("cr", NULL), 0, ft_charput);
	tputs(tgetstr("sf", NULL), 0, ft_charput);
	zero_cursor();
	return (1);
}

int				handle_controls(unsigned long code)
{
	int ret;
	int	cursor_pos;

	ret = 0;
	cursor_pos = calc_termsize();
	if (code == DELETE)
	{
		if (cursor_pos >= 0)
		{
			t_buff_line_rm(g_term.curr_buff, cursor_pos, 1);
			reprint_buffer(g_term.curr_buff);
			termcap_reset_cursor(cursor_pos - 1,
					ft_strlen(g_term.curr_buff->buff_str));
		}
	}
	else if (code == ENTER)
		handle_enter();
	else if (code == UP || code == DOWN || code == LEFT || code == RIGHT)
		handle_arrows(code);
	else
		ret -= 1;
	ret += 1;
	return (ret);
}
