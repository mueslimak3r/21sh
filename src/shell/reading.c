/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reading.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 19:32:39 by calamber          #+#    #+#             */
/*   Updated: 2019/12/06 23:12:17 by calamber         ###   ########.fr       */
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

int			interpret_input(int hd, t_input *thing, char *buf, t_tbuff *tbuff)
{
	int cursor_pos;

	ft_memcpy(thing->arr_form, buf, 4);
	if ((handle_controls(thing->long_form)) < 1)
	{
		cursor_pos = (g_term.conf.cursor[1] * g_term.conf.termsize[0]) + g_term.conf.cursor[0] - g_term.conf.prompt_size;
		tbuff_line_insert(tbuff, buf, cursor_pos);
		reprint_buffer(tbuff);
		//ft_printf_fd(STDERR_FILENO, "x %d y %d p: %d ps: %d", g_term.conf.cursor[0], g_term.conf.cursor[1], cursor_pos, g_term.conf.prompt_size);
		termcap_reset_cursor(cursor_pos, ft_strlen(tbuff->buff_str));
	}
	else if (thing->long_form == ENTER && !hd)
		return (1);
	ft_memset(buf, 0, BUFF_SIZE + 1);
	return (0);
}

void		print_prompt(int hd)
{
	char	*pwd;

	pwd = find_env("PWD");
	g_term.conf.prompt_size = hd ? 2 : ft_strlen(find_env("PWD")) + 1;
	//ft_printf_fd(STDERR_FILENO, "PS: |%d| hd: |%d|", g_term.conf.prompt_size, hd);
	ft_printf_fd(STDERR_FILENO, "%s>", pwd);
}

int			ft_readstdin_line(t_tbuff *tbuff, int hd)
{
	char	buf[BUFF_SIZE + 1];
	int		ret;
	t_input	thing;

	ft_memset(buf, 0, BUFF_SIZE + 1);
	print_prompt(hd);
	zero_cursor();
	//tputs(tgetstr("am", NULL), 0, ft_charput);
	thing.long_form = 0;
	ret = 0;
	while (!g_term.sigs.sigint)
	{
		if (!(check_fd(0) && ((ret = read(0, &buf, 4)) > 0)))
			continue ;
		if (buf[0] == 4)
		{
			if (!g_term.sigs.sigint && (!tbuff->buff_str))
			{
				close(0);
				ret = -1;
				break ;
			}
			buf[0] = 0;
		}
		if (interpret_input(hd, &thing, buf, tbuff))
			return (1);
	}
	return (ret);
}

int			get_input(void)
{
	int	ret;
	int res;

	ret = 0;
	if (!g_term.buff || (g_term.buff && g_term.buff->buff_str &&
										*(g_term.buff->buff_str)))
		tbuff_new(&g_term.buff);
	g_term.curr_buff = g_term.buff;
	if ((res = ft_readstdin_line(g_term.curr_buff, 0)) == 1)
		ret = 1;
	if (res < 0)
		ret = -1;
	else if (g_term.sigs.sigint || !g_term.curr_buff ||
		!g_term.curr_buff->buff_str || (g_term.curr_buff->buff_str &&
									!*(g_term.curr_buff->buff_str)))
		ret = 0;
	return (ret);
}
