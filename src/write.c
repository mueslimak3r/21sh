/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/20 00:37:55 by alkozma           #+#    #+#             */
/*   Updated: 2019/11/08 17:40:34 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

int		term_write(char *str, int fd, int cmd)
{
	int		des;

	des = fd;
	if (ft_strlen(str) < 1)
		return (1);
	if (!cmd)
	{
		if (g_term.conf.cursor[0] + ft_strlen(str) > g_term.conf.termsize[0])
		{
			//if (g_term.conf.termsize[0] - g_term.conf.cursor[0] > 0)
			//	write(des, str, g_term.conf.termsize[0] - g_term.conf.cursor[0]);
			write(des, str, ft_strlen(str));
			//if (ft_strlen(str) - g_term.conf.termsize[0] + g_term.conf.cursor[0] > 0)
			//	write(des, str + g_term.conf.termsize[0] - g_term.conf.cursor[0], ft_strlen(str) - g_term.conf.termsize[0] + g_term.conf.cursor[0]);
			//g_term.conf.cursor[0] = ft_strlen(str) - g_term.conf.termsize[0] + g_term.conf.cursor[0];
			g_term.conf.cursor[0] = ft_strlen(str);
			g_term.conf.cursor[1]++;
			g_term.conf.curlines++;
		}
		else
		{
			g_term.conf.cursor[0] += ft_strlen(str);
			write(des, str, ft_strlen(str));
		}
		//ft_putstr_fd("\u2588", STDERR_FILENO);
		//ft_putstr_fd("\b", STDERR_FILENO);
	}
	else
		write(des, str, ft_strlen(str));
	return (1);
}

/*
int		rem_from_buf(char *str, int pos)
{
	int	i;
	int	max;

	max = ft_strlen(str);
	i = 0;
	while (pos != i)
		i++;
	while (str[i])
	{
		str[i] = str[i + 1];
		i++;
	}
	str[i] = 0;
	return (1);
}

int		delete_char(char *str)
{
	char		buf[30];
	int			curpos;
	static int	broke_line;

	if (!str)
		return (1);
	if (ft_strlen(str) < 1)
		return (1);
	ft_printf_fd(STDERR_FILENO, " ");
	ft_putstr_fd("\b", STDERR_FILENO);
	g_term.conf.cursor[0] -= 1;
	rem_from_buf(str, (g_term.conf.cursor[0] + (g_term.conf.curlines - 1)) + ((g_term.conf.curlines - 1) * (g_window_size.ws_col - 1)) - 2);
	if (g_term.conf.cursor[0] < 0)
	{
		broke_line = 1;
		g_term.conf.cursor[0] = g_term.conf.termsize[0] - 1;
		g_term.conf.cursor[1]--;
		g_term.conf.curlines--;
		ft_printf_fd(STDERR_FILENO, "\033[1A");
		ft_printf_fd(STDERR_FILENO, "\033[%dC", g_term.conf.termsize[0]);
		ft_printf_fd(STDERR_FILENO, " ");
		ft_printf_fd(STDERR_FILENO, "\033[%dC", g_term.conf.termsize[0]);
	}
	else
	{
		if (broke_line)
			broke_line = 0;
		else
			ft_putstr_fd("\b", STDERR_FILENO);
		ft_putstr_fd(" \b", STDERR_FILENO);
	}
	return (1);
}
*/

int		ft_charput(int c)
{
	return (write(1, &c, 1));
}

int		reprint_buffer(t_tbuff *buff)
{
	//ft_printf_fd(STDERR_FILENO, "reprinting buff\ntermcursor h: %d y: %d\n", g_term.conf.cursor[0], g_term.conf.cursor[1]);
	tputs(tgetstr("dl", NULL), 0, ft_charput);
	tputs(tgetstr("cr", NULL), 0, ft_charput);
	ft_printf_fd(STDERR_FILENO, "%s", PROMPT);
	if (buff)
	{
		int size = sum_length(buff->rope);
		//if (buff->rope_buff[0] && buff->cursor - 1 < size)
		//term_write(buff->rope_buff, STDERR_FILENO, 1);
		//int i = 1;
		//t_rope_node *lleaf = rope_idx(buff->rope, &i);
		//rope_print_from_index(buff->rope, 1, buff->cursor);
		if (buff->rope)
			rope_print(buff->rope);
		//if (buff->rope_buff[0] && size <= buff->cursor - 1)
		//ft_printf_fd(STDERR_FILENO, "%s", buff->rope_buff);//term_write(buff->rope_buff, STDERR_FILENO, 1);
		//rope_print_from_index(buff->rope, buff->cursor + 1, size);
		tputs(tgetstr("cr", NULL), 0, ft_charput);
		for (int i = 0; i < g_term.conf.cursor[0]; i++)
			tputs(tgetstr("nd", NULL), 0, ft_charput);
	}
	return (0);
}

int     move_cursor(int amt)
{
	//ft_printf_fd(STDERR_FILENO, "move cursor\n");
    if (!g_term.curr_buff || g_term.conf.cursor[0] + amt > sum_length(g_term.curr_buff->rope) + 2)
        return (0);
    if (g_term.conf.cursor[0] + amt > g_term.conf.termsize[0])
    {
        g_term.conf.cursor[0] = amt;
        g_term.conf.cursor[1]++;
        g_term.conf.curlines++;
    }
    else if (g_term.conf.cursor[0] + amt < 0)
    {
        g_term.conf.cursor[0] = g_term.conf.termsize[0] - 1;
        g_term.conf.cursor[1]--;
        g_term.conf.curlines--;
    }
    else if (g_term.curr_buff && g_term.conf.cursor[0] + amt <= sum_length(g_term.curr_buff->rope) + 2)
        g_term.conf.cursor[0] += amt;
    return (1);
}

int		handle_controls(unsigned long code, char *str, char *saved)
{
	int ret = 0;

	if (code == DELETE)
	{
		;
	}
	else if (code == ENTER)
	{
		ft_printf_fd(STDERR_FILENO, " \b\n");
		g_term.conf.cursor[0] = ft_strlen(PROMPT);
		g_term.conf.cursor[1] = 0;
	}
	else if (code == TAB)
	{
		auto_complete();
	}
	else if (code == UP || code == DOWN || code == LEFT || code == RIGHT)
	{	
		if (code == UP)
		{
			if (g_term.curr_buff && g_term.curr_buff->next)
			{
				g_term.curr_buff = g_term.curr_buff->next;
				int len = sum_length(g_term.curr_buff->rope);
				g_term.curr_buff->cursor = len;
				if (len < 1)
				{
					g_term.conf.cursor[0] = 2;
					g_term.conf.cursor[1] = 0;
				}
				else
				{
					g_term.conf.cursor[0] = len % g_term.conf.termsize[0] + 2;
					g_term.conf.cursor[1] = len / g_term.conf.termsize[0];
				}
				reprint_buffer(g_term.curr_buff);
			}
		}
		if (code == DOWN)
		{
			if (g_term.curr_buff && g_term.curr_buff->prev)
			{
				g_term.curr_buff = g_term.curr_buff->prev;
				int len = sum_length(g_term.curr_buff->rope);
				g_term.curr_buff->cursor = len;
				if (len < 1)
				{
					g_term.conf.cursor[0] = 2;
					g_term.conf.cursor[1] = 0;
				}
				else
				{
					g_term.conf.cursor[0] = len % g_term.conf.termsize[0] + 2;
					g_term.conf.cursor[1] = len / g_term.conf.termsize[0];
				}
				reprint_buffer(g_term.curr_buff);
			}
		}
		if (code == LEFT || code == RIGHT)
		{
			int len = sum_length(g_term.curr_buff->rope);
			if (code == LEFT)
				g_term.curr_buff->cursor -= g_term.curr_buff->cursor < 1 ? 0 : 1;
			else
				g_term.curr_buff->cursor += g_term.curr_buff->cursor < len ? 1 : 0;
			move_cursor(code == LEFT ? -1 : 1);
			ft_printf_fd(STDERR_FILENO, "%s", str);
		}
	}
	else
		ret -= 1;
	ret += 1;
	//ft_printf_fd(STDERR_FILENO, "code: %lu\n", code);
	return (ret);
}
