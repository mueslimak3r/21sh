/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reading.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 19:32:39 by calamber          #+#    #+#             */
/*   Updated: 2019/12/19 16:47:32 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

int			interpret_input(char *buf, t_tbuff **tbuff)
{
	int		cursor_pos;
	t_input	thing;

	ft_memset(thing.arr_form, 0, sizeof(unsigned long));
	ft_memcpy(thing.arr_form, buf, sizeof(unsigned long));
	if ((handle_controls(thing.long_form, tbuff)) < 1 && ft_isprintable(buf))
	{
		cursor_pos = calc_pos();
		tbuff_line_insert(tbuff, buf, cursor_pos);
		reprint_buffer(*tbuff, cursor_pos, ft_strlen(buf));
	}
	else if (thing.long_form == KEY_ENTER)
		return (1);
	return (0);
}

void		handle_resize(t_tbuff *buff)
{
	int		pos;

	pos = calc_pos();
	if (g_term.conf.termsize[0] != g_window_size.ws_col ||
		g_term.conf.termsize[1] != g_window_size.ws_row)
	{
		zero_cursor(g_term.conf.prompt_size > 2 ? 0 : 1);
		move_cursor(pos, 0, buff, -1);
	}
}

int			readfromfd(t_tbuff **tbuff, int hd)
{
	char	buf[BUFF_SIZE + 1];
	int		ret;

	zero_cursor(hd);
	redo_prompt(hd, -1);
	ret = 0;
	while (!g_term.sigs.restart)
	{
		ft_memset(buf, 0, BUFF_SIZE + 1);
		if (!(check_fd(0) && ((ret = read(0, &buf, 4)) > 0)))
			continue ;
		handle_resize(*tbuff);
		if (buf[0] == FT_EOT)
		{
			if (!g_term.sigs.restart && (!(*tbuff)->buff_str))
			{
				hd ? 0 : close(0);
				ret = -1;
				return (-1);
			}
		}
		if (interpret_input(buf, tbuff))
			return (1);
	}
	return (g_term.sigs.restart ? -2 : ret);
}

int			ft_readstdin_line(t_tbuff **tbuff, int hd)
{
	int ret;

	ret = readfromfd(tbuff, hd);
	if (*tbuff && (*tbuff)->buff_str)
		move_cursor((*tbuff)->len - calc_pos(), 1, *tbuff, -1);
	tputs(tgetstr("cr", NULL), 0, ft_charput);
	tputs(tgetstr("sf", NULL), 0, ft_charput);
	if (ret == 1)
	{
		ret = tbuff_choose(tbuff, hd);
	}
	else
	{
		while (tbuff && *tbuff && (*tbuff)->next)
			*tbuff = (*tbuff)->next;
	}
	tbuff_cleanup(tbuff);
	tputs(tgetstr("me", NULL), 0, ft_charput);
	return (ret);
}

int			get_input(void)
{
	int	ret;

	ret = 0;
	tbuff_new(&g_term.buff);
	ret = ft_readstdin_line(&(g_term.buff), 0);
	if (ret == -1 || g_term.sigs.sigstop)
		return (-1);
	if (g_term.sigs.restart || !g_term.buff ||
		!g_term.buff->buff_str || (g_term.buff->buff_str &&
									!*(g_term.buff->buff_str)))
		ret = 0;
	return (ret);
}
