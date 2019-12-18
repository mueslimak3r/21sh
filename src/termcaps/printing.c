/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printing.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/13 18:38:28 by calamber          #+#    #+#             */
/*   Updated: 2019/12/18 11:06:37 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

int				calc_pos(void)
{
	return ((g_term.conf.cursor[1] * g_term.conf.termsize[0]) +
				g_term.conf.cursor[0] - g_term.conf.prompt_size);
}

void			reprint_buffer_helper(t_tbuff *buff, int index)
{
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
}

int				reprint_buffer(t_tbuff *buff, int pos, int move_amt)
{
	int		index;

	tputs(tgetstr("cr", NULL), 0, ft_charput);
	tputs(tgetstr("cd", NULL), 0, ft_charput);
	if (g_term.conf.cursor[1] == (g_term.conf.prompt_size /
								g_term.conf.termsize[0]))
	{
		redo_prompt(g_term.conf.prompt_size > 2 ? 0 : 1,
		g_term.conf.prompt_size % g_term.conf.termsize[0]);
	}
	if (buff && buff->buff_str)
	{
		tputs(tgetstr("vi", NULL), 0, ft_charput);
		index = (pos - g_term.conf.cursor[0] + ((g_term.conf.cursor[1]
			== (g_term.conf.prompt_size / g_term.conf.termsize[0])) ?
			g_term.conf.prompt_size % g_term.conf.termsize[0] : 0));
		g_term.conf.cursor[0] = ((g_term.conf.cursor[1] ==
			(g_term.conf.prompt_size / g_term.conf.termsize[0])) ?
			g_term.conf.prompt_size % g_term.conf.termsize[0] : 0);
		reprint_buffer_helper(buff, index);
		if (move_amt != 0)
			move_cursor(-(buff->len - pos) + move_amt, 1, buff, -1);
		tputs(tgetstr("ve", NULL), 0, ft_charput);
	}
	return (0);
}
