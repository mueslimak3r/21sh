/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkozma <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 20:10:40 by alkozma           #+#    #+#             */
/*   Updated: 2019/11/21 21:03:33 by alkozma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

int				term_write(char *str, int fd, int cmd)
{
	int		des;

	des = fd;
	if (ft_strlen(str) < 1)
		return (1);
	if (!cmd)
	{
		if (g_term.conf.cursor[0] + ft_strlen(str) >
				(unsigned int)g_term.conf.termsize[0])
		{
			write(des, str, ft_strlen(str));
			g_term.conf.cursor[0] = ft_strlen(str);
			g_term.conf.cursor[1]++;
			g_term.conf.curlines++;
		}
		else
		{
			g_term.conf.cursor[0] += ft_strlen(str);
			write(des, str, ft_strlen(str));
		}
	}
	else
		write(des, str, ft_strlen(str));
	return (1);
}

int				ft_charput(int c)
{
	return (write(1, &c, 1));
}

int				reprint_buffer(t_tbuff *buff)
{
	int		index;

	tputs(tgetstr("cr", NULL), 0, ft_charput);
	tputs(tgetstr("cd", NULL), 0, ft_charput);
	if (g_term.conf.cursor[1] == 0)
		ft_printf_fd(STDERR_FILENO, "%s", PROMPT);
	if (buff)
	{
		index = (g_term.conf.cursor[1] * g_term.conf.termsize[0]) - 1;
		if (index <= 0)
			index = 0;
		else if (index - PROMPT_SIZE >= 0)
			index -= PROMPT_SIZE;
		else
			index = 0;
		if (buff->buff_str)
			ft_printf_fd(STDERR_FILENO, "%s", buff->buff_str +
					(g_term.conf.cursor[1] > 0 ? index : 0));
	}
	return (0);
}

static int		handle_cursor(int amt, int dir)
{
	int		cursor[2];
	int		termsize[2];

	cursor[0] = g_term.conf.cursor[0];
	cursor[1] = g_term.conf.cursor[1];
	termsize[0] = g_term.conf.termsize[0];
	termsize[1] = g_term.conf.termsize[1];
	if (dir == 0)
	{
		g_term.conf.cursor[1] += (cursor[0] + amt) / termsize[0];
		g_term.conf.cursor[0] = (cursor[0] + (amt % termsize[0])) % termsize[0];
	}
	else
	{
		g_term.conf.cursor[1] -= ((cursor[0] + amt < 0)
				? -(cursor[0] + amt) : (cursor[0] + amt) / termsize[0]);
		g_term.conf.cursor[0] = (cursor[0] + (-amt % termsize[0]));
		g_term.conf.cursor[0] = (g_term.conf.cursor[0] < 0)
			? -(g_term.conf.cursor[0]) : g_term.conf.cursor[0];
		g_term.conf.cursor[0] = g_term.conf.cursor[0] % termsize[0];
	}
	g_term.conf.curlines = g_term.conf.cursor[1] + 1;
	return (1);
}

static int		calc_termsize(void)
{
	return ((g_term.conf.cursor[1] * g_term.conf.termsize[0])
			+ g_term.conf.cursor[0] - 1);
}

static int		handle_tc(int amt)
{
	int	i;

	i = 0;
	if (g_term.conf.cursor[0] + amt >= g_term.conf.termsize[0])
	{
		tputs(tgetstr("cr", NULL), 0, ft_charput);
		while (i++ < (g_term.conf.cursor[0] + amt) / g_term.conf.termsize[0])
			tputs(tgetstr("sf", NULL), 0, ft_charput);
		i = 0;
		while (++i < amt % g_term.conf.termsize[0])
			tputs(tgetstr("nd", NULL), 0, ft_charput);
	}
	else
	{
		tputs(tgetstr("cr", NULL), 0, ft_charput);
		while (i++ < -(g_term.conf.cursor[0] + amt) / g_term.conf.termsize[0])
			tputs(tgetstr("up", NULL), 0, ft_charput);
		i = 0;
		while (++i < -(g_term.conf.cursor[0] + amt) % g_term.conf.termsize[0])
			tputs(tgetstr("nd", NULL), 0, ft_charput);
	}
	return (1);
}

int				move_cursor(int amt, int affect_tc)
{
	int size;
	int	i;

	i = 0;
	size = calc_termsize();
	if (amt == 0 || !g_term.curr_buff ||
			(g_term.conf.cursor[0] + amt < 2 && g_term.conf.cursor[1] == 0))
		return (0);
	else if (g_term.conf.cursor[0] + amt >= g_term.conf.termsize[0])
	{
		if (affect_tc)
			handle_tc(amt);
		handle_cursor(amt, 0);
	}
	else if (g_term.conf.cursor[0] + amt < 0)
	{
		if (affect_tc)
			handle_tc(amt);
		handle_cursor(amt, 1);
	}
	else if (g_term.curr_buff && size - PROMPT_SIZE
			<= (int)ft_strlen(g_term.curr_buff->buff_str))
		g_term.conf.cursor[0] += amt;
	return (1);
}

static int		handle_up_down(int code)
{
	int		i;
	int		len;

	i = g_term.conf.curlines;
	if (code == DOWN && (!g_term.curr_buff || !g_term.curr_buff->prev))
		return (0);
	if (g_term.curr_buff && g_term.curr_buff->next)
	{
		tputs(tgetstr("cr", NULL), 0, ft_charput);
		while (i-- > 1)
			tputs(tgetstr("sr", NULL), 0, ft_charput);
		tputs(tgetstr("cd", NULL), 0, ft_charput);
		g_term.curr_buff = code == UP
			? g_term.curr_buff->next : g_term.curr_buff->prev;
		len = g_term.curr_buff->len;
		g_term.conf.cursor[0] = PROMPT_SIZE;
		g_term.conf.cursor[1] = 0;
		g_term.conf.curlines = 1;
		reprint_buffer(g_term.curr_buff);
		move_cursor(len, 0);
	}
	return (1);
}

static int		handle_arrows(int code)
{
	if (code == UP || code == DOWN)
		return (handle_up_down(code));
	if ((code == LEFT && !(g_term.conf.cursor[0] - 1 < 2
		&& g_term.conf.cursor[1] == 0))
		|| (code == RIGHT
			&& !(((g_term.conf.cursor[1] * g_term.conf.termsize[0]) +
			g_term.conf.cursor[0] - PROMPT_SIZE) >= g_term.curr_buff->len)))
	{
		tputs(tgetstr(code == LEFT ? "le" : "nd", NULL), 0, ft_charput);
		move_cursor(code == LEFT ? -1 : 1, 1);
	}
	return (0);
}

static int		zero_cursor(void)
{
	g_term.conf.cursor[0] = PROMPT_SIZE;
	g_term.conf.cursor[1] = 0;
	g_term.conf.curlines = 1;
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
	cursor_pos = calc_termsize() - PROMPT_SIZE;
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
