
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
	if (g_term.conf.cursor[1] == (g_term.conf.prompt_size / g_term.conf.termsize[0]))
	{
		redo_prompt(g_term.conf.prompt_size > 2 ? 0 : 1, g_term.conf.prompt_size % g_term.conf.termsize[0]);
	}
	if (buff && buff->buff_str)
	{
		index = (pos -
		g_term.conf.cursor[0] + ((g_term.conf.cursor[1] == (g_term.conf.prompt_size / g_term.conf.termsize[0])) ? g_term.conf.prompt_size % g_term.conf.termsize[0] : 0));
		g_term.conf.cursor[0] = ((g_term.conf.cursor[1] == (g_term.conf.prompt_size / g_term.conf.termsize[0])) ? g_term.conf.prompt_size % g_term.conf.termsize[0] : 0);

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
	return (1);
}
