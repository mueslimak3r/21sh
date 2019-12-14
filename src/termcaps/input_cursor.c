/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_cursor.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/13 18:18:52 by calamber          #+#    #+#             */
/*   Updated: 2019/12/13 18:55:52 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

int				ft_abs(int amt)
{
	return (amt < 0 ? amt * -1 : amt);
}

static void		handle_tc_back(int use_tc)
{
	int	j;

	if (use_tc)
	{
		j = 0;
		tputs(tgetstr("cr", NULL), 0, ft_charput);
		tputs(tgetstr("up", NULL), 0, ft_charput);
		while (j < g_term.conf.termsize[0])
		{
			tputs(tgetstr("nd", NULL), 0, ft_charput);
			j++;
		}
	}
	g_term.conf.cursor[0] = g_term.conf.termsize[0] - 1;
	g_term.conf.cursor[1]--;
}

static void		handle_tc_forward(int i, int pos, int use_tc, t_tbuff *buff)
{
	while (i > 0 && pos <= buff->len)
	{
		if (g_term.conf.cursor[0] >= g_term.conf.termsize[0] - 1)
		{
			if (use_tc)
			{
				tputs(tgetstr("cr", NULL), 0, ft_charput);
				tputs(tgetstr("do", NULL), 0, ft_charput);
			}
			g_term.conf.cursor[0] = 0;
			g_term.conf.cursor[1]++;
		}
		else
		{
			if (use_tc)
				tputs(tgetstr("nd", NULL), 0, ft_charput);
			g_term.conf.cursor[0]++;
		}
		i--;
		pos++;
	}
}

static int		handle_tc(int amt, int pos, int use_tc, t_tbuff *buff)
{
	int dir;
	int i;

	dir = (amt < 0) ? 0 : 1;
	amt = ft_abs(amt);
	i = amt;
	while (!dir && i > 0 && pos >= 0)
	{
		if (g_term.conf.cursor[0] <= 0)
			handle_tc_back(use_tc);
		else
		{
			if (use_tc)
				tputs(tgetstr("le", NULL), 0, ft_charput);
			g_term.conf.cursor[0]--;
		}
		i--;
		pos--;
	}
	if (dir)
		handle_tc_forward(amt, pos, use_tc, buff);
	return (1);
}

int				move_cursor(int amt, int affect_tc, t_tbuff *buff, int pos)
{
	int size;

	pos = (pos != -1) ? pos : calc_pos();
	size = buff->len;
	if (amt == 0 || !buff->buff_str ||
			(g_term.conf.cursor[1] == 0 && size + amt < 0))
	{
		return (0);
	}
	if (pos + amt >= 0 && pos + amt <= size)
		handle_tc(amt, pos, affect_tc, buff);
	return (1);
}
