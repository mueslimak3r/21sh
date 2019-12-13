/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reading.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 19:32:39 by calamber          #+#    #+#             */
/*   Updated: 2019/12/13 15:05:24 by alkozma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

int			check_fd(int fd)
{
	fd_set			set;
	struct timeval	timeout;
	int				rv;

	FD_ZERO(&set);
	FD_SET(fd, &set);
	timeout.tv_sec = 0;
	timeout.tv_usec = 10000;
	rv = select(fd + 1, &set, NULL, NULL, &timeout);
	if (rv == -1)
		return (0);
	else if (rv == 0)
		return (0);
	else
		return (1);
	return (0);
}

int			interpret_input(int hd, char *buf, t_tbuff **tbuff)
{
	int cursor_pos;
	t_input thing;

	ft_memset(thing.arr_form, 0, sizeof(unsigned long));
	ft_memcpy(thing.arr_form, buf, sizeof(unsigned long));
	if ((handle_controls(thing.long_form, tbuff)) < 1)
	{
		cursor_pos = calc_pos();
		tbuff_line_insert(tbuff, buf, cursor_pos);
		reprint_buffer(*tbuff, cursor_pos, ft_strlen(buf));
		//move_cursor(ft_strlen(buf), 0, *tbuff);
		//ft_printf_fd(STDERR_FILENO, "x %d y %d p: %d ps: %d", g_term.conf.cursor[0], g_term.conf.cursor[1], cursor_pos, g_term.conf.prompt_size);
		//termcap_reset_cursor(cursor_pos, ft_strlen((*tbuff)->buff_str));
	}
	else if (thing.long_form == KEY_ENTER && (hd || !hd))
		return (1);
	return (0);
}

void		redo_prompt(int hd, int print)
{
	char	*pwd;
	int		len;
	int		start;
	int		end;

	pwd = find_env("PWD");
	g_term.conf.prompt_size = hd ? 2 : ft_strlen(find_env("PWD")) + 1;
	if (print || print == -1)
	{
		len = hd ? 2 : ft_strlen(pwd) + 1;
		print = print == -1 ? len : print;
		pwd = hd ? " >" : pwd;
		start = len - print;
		end = start + print;
		while (start < end)
		{
			if (start == end - 1 && !hd)
				ft_putchar('>');
			else
				ft_putchar(pwd[start]);
			start++;
		}
	}
	//ft_printf_fd(STDERR_FILENO, "PS: |%d| hd: |%d|", g_term.conf.prompt_size, hd);
	
}

void		handle_resize(t_tbuff *buff)
{
	int		pos;
	int		i;

	i = 1;
	//pos = (g_term.conf.cursor[1] * g_term.conf.termsize[0]) + g_term.conf.cursor[0]; 
	pos = calc_pos();
	if (g_term.conf.termsize[0] != g_window_size.ws_col ||
		g_term.conf.termsize[1] != g_window_size.ws_row)
	{
		if (buff)
		{
			
		}
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
				g_term.pid == 0 ? close(0) : 0;
				ret = -1;
				return (-1);
			}
			buf[0] = 0;
		}
		if (interpret_input(hd, buf, tbuff))
			return (1);
	}
	ret = g_term.sigs.restart ? -2 : ret;
	return (ret);
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
	//ft_printf_fd(STDERR_FILENO, "done\n");
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
	g_term.sigs.restart = false;
	g_term.sigs.sigint = false;
	return (ret);
}
