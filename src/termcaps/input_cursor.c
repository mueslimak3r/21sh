
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_cursor.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 20:10:40 by alkozma           #+#    #+#             */
/*   Updated: 2019/12/08 02:40:09 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

int				ft_abs(int amt)
{
	return (amt < 0 ? amt * -1 : amt);
}

int				reprint_buffer(t_tbuff *buff, int pos, int move_amt)
{
	int		index;

	tputs(tgetstr("cr", NULL), 0, ft_charput);
	tputs(tgetstr("cd", NULL), 0, ft_charput);
	if (g_term.conf.cursor[1] == 0)
		redo_prompt(g_term.conf.prompt_size > 2 ? 0 : 1, 1);
	if (buff && buff->buff_str)
	{
		index = (pos -
		g_term.conf.cursor[0] + (g_term.conf.cursor[1] == 0 ? g_term.conf.prompt_size : 0));
		g_term.conf.cursor[0] = g_term.conf.cursor[1] ? 0 : g_term.conf.prompt_size;
		while (index < buff->len)
		{
			ft_charput(buff->buff_str[index]);
			index++;
			g_term.conf.cursor[0]++;
			if (g_term.conf.cursor[0] >= g_term.conf.termsize[0])
			{
				ft_putstr(" \b");
				g_term.conf.cursor[0] = 0;
				g_term.conf.cursor[1]++;
			}
		}
		if (move_amt != 0)
			move_cursor(-(buff->len - pos) + move_amt, 1, buff, -1);
	}
	return (0);
}
/*
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
		g_term.conf.cursor[0] = (cursor[0] + amt) % termsize[0];
	}
	else
	{
		g_term.conf.cursor[1] -= 1 + (-amt / termsize[0]);
		g_term.conf.cursor[0] = amt == -1 ? termsize[0] - 1 : -amt % termsize[0];
	}
	g_term.conf.curlines = g_term.conf.cursor[1] + 1;
	return (1);
}
*/

int				calc_pos(void)
{
	return ((g_term.conf.cursor[1] * g_term.conf.termsize[0]) + g_term.conf.cursor[0] - g_term.conf.prompt_size);
}

static int		handle_tc(int amt, int pos, int use_tc, t_tbuff *buff)
{
	int	i;
	int dir;

	if (buff)
	{
		;
	}
	dir = (amt < 0) ? 0 : 1;
	amt = ft_abs(amt);
	i = amt;
	while (!dir && i > 0 && pos >= 0)
	{
		if (g_term.conf.cursor[0] <= 0)
		{
			if (use_tc)
			{
				tputs(tgetstr("cr", NULL), 0, ft_charput);
				tputs(tgetstr("up", NULL), 0, ft_charput);
				for (int j = 0; j < g_term.conf.termsize[0]; j++)
					tputs(tgetstr("nd", NULL), 0, ft_charput);
			}
			i--;
			g_term.conf.cursor[0] = g_term.conf.termsize[0] - 1;
			g_term.conf.cursor[1]--;
		}
		else
		{
			if (use_tc)
				tputs(tgetstr("le", NULL), 0, ft_charput);
			g_term.conf.cursor[0]--;
			i--;
		}
		pos--;
	}
	while (dir && i > 0 && pos <= buff->len)
	{
		if (g_term.conf.cursor[0] >= g_term.conf.termsize[0] - 1)
		{
			if (use_tc)
			{
				
				//if (buff->len + g_term.conf.prompt_size + 1 % g_term.conf.termsize[0] == 0)
				
				tputs(tgetstr("cr", NULL), 0, ft_charput);
				tputs(tgetstr("do", NULL), 0, ft_charput);
			}
			i--;
			//tputs(tgetstr("nw", NULL), 0, ft_charput);
			g_term.conf.cursor[0] = 0;
			g_term.conf.cursor[1]++;
		}
		else
		{
			if (use_tc)
				tputs(tgetstr("nd", NULL), 0, ft_charput);
			g_term.conf.cursor[0]++;
			i--;
		}
		pos++;
	}
	/*
	else
	{
		tputs(tgetstr("cr", NULL), 0, ft_charput);
		while (i++ <= -(g_term.conf.cursor[0] + amt) / g_term.conf.termsize[0])
			tputs(tgetstr("", NULL), 0, ft_charput);
		i = 0;
		while (++i <= g_term.conf.termsize[0] - (-(g_term.conf.cursor[0] + amt) % g_term.conf.termsize[0]))
			tputs(tgetstr("nd", NULL), 0, ft_charput);
	}
	*/
	return (1);
}

int				move_cursor(int amt, int affect_tc, t_tbuff *buff, int pos)
{
	int size;

	pos = (pos != -1) ? pos : calc_pos();
	size = buff->len;
	//ft_printf_fd(STDERR_FILENO, "s: %d", size);
	if (amt == 0 || !buff->buff_str ||
			(g_term.conf.cursor[1] == 0 && size + amt < 0))
	{
		return (0);
	}
	if (affect_tc)
	{
		;
	}
	if (pos + amt >= 0 && pos + amt <= size)
		handle_tc(amt, pos, affect_tc, buff);
	/*
	else if (g_term.conf.cursor[0] + amt >= g_term.conf.termsize[0])
	{
		//int tmpp[2] = { g_term.conf.cursor[0], g_term.conf.cursor[1] };
		if (affect_tc)
		{
			handle_tc(amt, 1);
		}
		handle_cursor(amt, 0);
		
		ft_printf_fd(STDERR_FILENO, "x|%d|y|%d||x|%d|y|%d", tmpp[0], tmpp[1], g_term.conf.cursor[0], g_term.conf.cursor[1]);
		while (1)
		{
			;
		}
		
	}
	else if (g_term.conf.cursor[0] + amt < 0)
	{
		if (affect_tc)
		{
			handle_tc(amt, 0);
		}
		else
			handle_cursor(amt, 1);
	}
	else if (buff->buff_str && ((amt > 0 && g_term.conf.cursor[0] + amt < g_term.conf.termsize[0]) || (amt < 0 && size + amt >= 0)))
	{
		if (affect_tc)
		{
			handle_tc(amt);
		}
		else
			g_term.conf.cursor[0] += amt;
	}
	*/
	return (1);
}
