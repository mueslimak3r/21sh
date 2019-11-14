/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/20 00:37:55 by alkozma           #+#    #+#             */
/*   Updated: 2019/11/14 05:08:06 by calamber         ###   ########.fr       */
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
		if (g_term.conf.cursor[0] + ft_strlen(str) > (unsigned int)g_term.conf.termsize[0])
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

int		ft_charput(int c)
{
	return (write(1, &c, 1));
}

int		reprint_buffer(t_tbuff *buff)
{
	tputs(tgetstr("cr", NULL), 0, ft_charput);
	tputs(tgetstr("cd", NULL), 0, ft_charput);
	if (g_term.conf.cursor[1] == 0)
	{
		ft_printf_fd(STDERR_FILENO, "%s", PROMPT);
	}
	if (buff)
	{
		int index;
		index = (g_term.conf.cursor[1] * g_term.conf.termsize[0]) - 1;
		if (index <= 0)
			index = 0;
		else if (index - PROMPT_SIZE >= 0)
			index -= PROMPT_SIZE;
		else
			index = 0;
		//ft_printf_fd(STDERR_FILENO, "p from %d\n", index);
		if (buff->buff_str)
			ft_printf_fd(STDERR_FILENO, "%s", buff->buff_str +
					(g_term.conf.cursor > 0 ? index : 0));
	}
	tputs(tgetstr("cr", NULL), 0, ft_charput);
	for (int i = 0; i < g_term.conf.cursor[0]; i++)
			tputs(tgetstr("nd", NULL), 0, ft_charput);
	return (0);
}

int     move_cursor(int amt)
{
    int size = (g_term.conf.cursor[1] * g_term.conf.termsize[0]) + g_term.conf.cursor[0] - 1;

	if (!g_term.curr_buff || (g_term.conf.cursor[0] + amt < 2 && g_term.conf.cursor[1] == 0))
        return (0);
    else if (g_term.conf.cursor[0] + amt >= g_term.conf.termsize[0])
    {
		tputs(tgetstr("cr", NULL), 0, ft_charput);
		for (int i = 0; i < amt / g_term.conf.termsize[0]; i++)
			tputs(tgetstr("do", NULL), 0, ft_charput);
		for (int i = 1; i < amt % g_term.conf.termsize[0]; i++)
			tputs(tgetstr("nd", NULL), 0, ft_charput);
        g_term.conf.cursor[0] = (g_term.conf.cursor[0] + (amt % g_term.conf.termsize[0])) % g_term.conf.termsize[0];
        g_term.conf.cursor[1] += amt / g_term.conf.termsize[0];
        g_term.conf.curlines= g_term.conf.cursor[1];
    }
    else if (g_term.conf.cursor[0] + amt < 0)
    {
		tputs(tgetstr("cr", NULL), 0, ft_charput);
		for (int i = 0; i < (-amt) / g_term.conf.termsize[0]; i++)
			tputs(tgetstr("up", NULL), 0, ft_charput);
		for (int i = 1; i < (-amt) % g_term.conf.termsize[0]; i++)
			tputs(tgetstr("nd", NULL), 0, ft_charput);
        g_term.conf.cursor[0] = (g_term.conf.cursor[0] - (amt % g_term.conf.termsize[0]));
		g_term.conf.cursor[0] = (g_term.conf.cursor[0] < 0) ? -(g_term.conf.cursor[0]) : g_term.conf.cursor[0];
		g_term.conf.cursor[0] = g_term.conf.cursor[0] % g_term.conf.termsize[0];
        g_term.conf.cursor[1] -= amt / g_term.conf.termsize[0];
        g_term.conf.curlines = g_term.conf.cursor[1];
    }
    else if (g_term.curr_buff && size - PROMPT_SIZE <= (int)ft_strlen(g_term.curr_buff->buff_str))
        g_term.conf.cursor[0] += amt;
    return (1);
}

int		handle_controls(unsigned long code, char *str)
{
	int ret = 0;

	if (code == DELETE)
	{
		// delete char
		int cursor_pos = (g_term.conf.cursor[1] * g_term.conf.termsize[0]) + g_term.conf.cursor[0] - PROMPT_SIZE - 1;
		t_buff_line_rm(g_term.curr_buff, cursor_pos, 1);
		reprint_buffer(g_term.curr_buff);
	}
	else if (code == ENTER)
	{
		//
		// set cursor to end of buff line
		//
		if (g_term.curr_buff && g_term.curr_buff->buff_str)
		{
			int len = g_term.curr_buff->len;
			while (g_term.conf.curlines > 1)
			{
				tputs(tgetstr("cr", NULL), 0, ft_charput);
				tputs(tgetstr("cd", NULL), 0, ft_charput);
				g_term.conf.curlines--;
			}
			g_term.conf.cursor[0] = PROMPT_SIZE;
			g_term.conf.cursor[1] = 0;
			g_term.conf.curlines = 1;
			move_cursor(len);
			reprint_buffer(g_term.curr_buff);
		}
		ft_printf_fd(STDERR_FILENO, " \b\n");
		g_term.conf.cursor[0] = PROMPT_SIZE;
		g_term.conf.cursor[1] = 0;
		g_term.conf.curlines = 1;
	}
	else if (code == TAB)
	{
		auto_complete();
	}
	else if (code == UP || code == DOWN || code == LEFT || code == RIGHT)
	{
		int len;
		if (code == UP)
		{
			//ft_printf_fd(STDERR_FILENO, "up\n");
			if (g_term.curr_buff && g_term.curr_buff->next)
			{
				g_term.curr_buff = g_term.curr_buff->next;
				len = g_term.curr_buff->len;
				while (g_term.conf.curlines > 1)
				{
					tputs(tgetstr("cr", NULL), 0, ft_charput);
					tputs(tgetstr("cd", NULL), 0, ft_charput);
					g_term.conf.curlines--;
				}
				g_term.conf.cursor[0] = PROMPT_SIZE;
				g_term.conf.cursor[1] = 0;
				g_term.conf.curlines = 1;
				move_cursor(len);
				reprint_buffer(g_term.curr_buff);
			}
		}
		if (code == DOWN)
		{
			if (g_term.curr_buff && g_term.curr_buff->prev)
			{
				g_term.curr_buff = g_term.curr_buff->prev;
				len = g_term.curr_buff->len;
				while (g_term.conf.curlines > 1)
				{
					tputs(tgetstr("cr", NULL), 0, ft_charput);
					tputs(tgetstr("cd", NULL), 0, ft_charput);
					g_term.conf.curlines--;
				}
				g_term.conf.cursor[0] = PROMPT_SIZE;
				g_term.conf.cursor[1] = 0;
				g_term.conf.curlines = 1;
				move_cursor(len);
				reprint_buffer(g_term.curr_buff);
			}
		}
		if (code == LEFT || code == RIGHT)
		{
			if ((code == LEFT && !(g_term.conf.cursor[0] - 1 < 2 && g_term.conf.cursor[1] == 0)) ||
				(code == RIGHT && !(((g_term.conf.cursor[1] * g_term.conf.termsize[0]) + g_term.conf.cursor[0] - PROMPT_SIZE + 1) > g_term.curr_buff->len)))
			{
				ft_printf_fd(STDERR_FILENO, "%s", str);
				move_cursor(code == LEFT ? -1 : 1);
			}
		}
	}
	else
		ret -= 1;
	ret += 1;
	//ft_printf_fd(STDERR_FILENO, "code: %lu\n", code);
	return (ret);
}
