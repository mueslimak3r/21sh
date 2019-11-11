/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/20 00:37:55 by alkozma           #+#    #+#             */
/*   Updated: 2019/11/10 22:16:50 by calamber         ###   ########.fr       */
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

int		ft_charput(int c)
{
	return (write(1, &c, 1));
}

int		reprint_buffer(t_tbuff *buff)
{
	//ft_printf_fd(STDERR_FILENO, "reprinting buff\ntermcursor h: %d y: %d\n", g_term.conf.cursor[0], g_term.conf.cursor[1]);
	tputs(tgetstr("cr", NULL), 0, ft_charput);
	tputs(tgetstr("cd", NULL), 0, ft_charput);
	if (g_term.conf.cursor[1] == 0)
	{
		ft_printf_fd(STDERR_FILENO, "%s", PROMPT);
	}
	if (buff)
	{
		int size = sum_length(buff->rope);
		//if (buff->rope_buff[0] && buff->cursor - 1 < size)
		//term_write(buff->rope_buff, STDERR_FILENO, 1);
		//int i = 1;
		//t_rope_node *lleaf = rope_idx(buff->rope, &i);
		//rope_print_from_index(buff->rope, 1, buff->cursor);
		int index;
		index = (g_term.conf.cursor[1] * g_term.conf.termsize[0]) - 1;
		if (index <= 0)
			index = 0;
		else if (index - PROMPT_SIZE >= 0)
			index -= PROMPT_SIZE;
		else
			index = 0;
		//int cursor_size = (g_term.conf.cursor[0] == 0 && index - 1 > 0) ? 1 : 0;
		//ft_printf_fd(STDERR_FILENO, "index: %d rindex: %d\n", index, g_term.curr_buff->cursor);
		if (buff->rope)
		{
			if (g_term.conf.cursor[1] > 0)
			{
				//ft_printf_fd(STDERR_FILENO, "tsx %d tsy: %d | i: %d s: %d", g_term.conf.termsize[0], g_term.conf.termsize[1], index, size);
				rope_print_from_index(buff->rope, index + 1, size);
			}
			//rope_print_from_index(buff->rope, 1, size);
			else
				rope_print(buff->rope);
		}
		//if (buff->rope_buff[0] && size <= buff->cursor - 1)
		//ft_printf_fd(STDERR_FILENO, "%s", buff->rope_buff);//term_write(buff->rope_buff, STDERR_FILENO, 1);
		//rope_print_from_index(buff->rope, buff->cursor + 1, size);
	}
	tputs(tgetstr("cr", NULL), 0, ft_charput);
	for (int i = 0; i < g_term.conf.cursor[0]; i++)
			tputs(tgetstr("nd", NULL), 0, ft_charput);
	return (0);
}

int     move_cursor(int amt)
{
	//ft_printf_fd(STDERR_FILENO, "move cursor\n");
    int size = (g_term.conf.cursor[1] * g_term.conf.termsize[0]) + g_term.conf.cursor[0] - 1;

	if (!g_term.curr_buff || (g_term.conf.cursor[0] + amt < 2 && g_term.conf.cursor[1] == 0))
        return (0);
    else if (g_term.conf.cursor[0] + amt >= g_term.conf.termsize[0])
    {
		//tputs(tgetstr("cr", NULL), 0, ft_charput);
		//tputs(tgetstr("al", NULL), 0, ft_charput);
		tputs(tgetstr("do", NULL), 0, ft_charput);
        g_term.conf.cursor[0] = amt;
        g_term.conf.cursor[1]++;
        g_term.conf.curlines++;
		//ft_printf_fd(STDERR_FILENO, "gsize x: %d y: %d\n", g_term.conf.cursor[0], g_term.conf.cursor[1]);
    }
    else if (g_term.conf.cursor[0] + amt < 0)
    {
		tputs(tgetstr("up", NULL), 0, ft_charput);
		for (int i = 1; i < g_term.conf.termsize[0]; i++)
			tputs(tgetstr("nd", NULL), 0, ft_charput);
        g_term.conf.cursor[0] = g_term.conf.termsize[0] - 1;
        g_term.conf.cursor[1]--;
        g_term.conf.curlines--;
    }
    else if (g_term.curr_buff && size - PROMPT_SIZE <= sum_length(g_term.curr_buff->rope))
        g_term.conf.cursor[0] += amt;
    return (1);
}

int		handle_controls(unsigned long code, char *str)
{
	int ret = 0;

	if (code == DELETE)
	{
		;
	}
	else if (code == ENTER)
	{
		ft_printf_fd(STDERR_FILENO, " \b\n");
		//ft_printf_fd(STDERR_FILENO, "gtsize x: %d y: %d\n", g_term.conf.termsize[0], g_term.conf.termsize[1]);
		//ft_printf_fd(STDERR_FILENO, "len: %d, gsize x: %d y: %d\n", sum_length(g_term.curr_buff->rope) + 2, g_term.conf.cursor[0], g_term.conf.cursor[1]);
		g_term.curr_buff->cursor = sum_length(g_term.curr_buff->rope);
		g_term.conf.cursor[0] = ft_strlen(PROMPT);
		g_term.conf.cursor[1] = 0;
		g_term.conf.curlines = 1;
	}
	else if (code == TAB)
	{
		auto_complete();
	}
	else if (code == UP || code == DOWN || code == LEFT || code == RIGHT)
	{	
		if (code == UP)
		{
			//ft_printf_fd(STDERR_FILENO, "up\n");
			if (g_term.curr_buff && g_term.curr_buff->next)
			{
				g_term.curr_buff = g_term.curr_buff->next;
				g_term.curr_buff->cursor = sum_length(g_term.curr_buff->rope);
				while (g_term.conf.curlines)
				{
					tputs(tgetstr("cr", NULL), 0, ft_charput);
					tputs(tgetstr("cd", NULL), 0, ft_charput);
					g_term.conf.curlines--;
				}
				g_term.conf.curlines = 1;
				int len = sum_length(g_term.curr_buff->rope) + PROMPT_SIZE;
				g_term.curr_buff->cursor = len;
				if (len < 1)
				{
					g_term.conf.cursor[0] = 2;
					g_term.conf.cursor[1] = 0;
				}
				else
				{
					g_term.conf.cursor[1] = len / g_term.conf.termsize[0];
					g_term.conf.cursor[0] = (len % g_term.conf.termsize[0]) + PROMPT_SIZE; 
				}
				//ft_printf_fd(STDERR_FILENO, "gtsize x: %d y: %d\n", g_term.conf.termsize[0], g_term.conf.termsize[1]);
				//ft_printf_fd(STDERR_FILENO, "len: %d, msize x: %d y: %d\n", len, g_term.conf.cursor[0], g_term.conf.cursor[1]);
				ft_printf_fd(STDERR_FILENO, "%s", PROMPT);
				rope_print(g_term.curr_buff->rope);

				//reprint_buffer(g_term.curr_buff);
			}
		}
		if (code == DOWN)
		{
			if (g_term.curr_buff && g_term.curr_buff->prev)
			{
				g_term.curr_buff = g_term.curr_buff->prev;
				g_term.curr_buff->cursor = sum_length(g_term.curr_buff->rope);
				while (g_term.conf.curlines)
				{
					tputs(tgetstr("cr", NULL), 0, ft_charput);
					tputs(tgetstr("cd", NULL), 0, ft_charput);
					g_term.conf.curlines--;
				}
				g_term.conf.curlines = 1;
				int len = sum_length(g_term.curr_buff->rope) + PROMPT_SIZE;
				g_term.curr_buff->cursor = len;
				if (len < 1)
				{
					g_term.conf.cursor[0] = 2;
					g_term.conf.cursor[1] = 0;
				}
				else
				{
					g_term.conf.cursor[1] = len / g_term.conf.termsize[0];
					g_term.conf.cursor[0] = (len % g_term.conf.termsize[0]) + PROMPT_SIZE; 
				}
				ft_printf_fd(STDERR_FILENO, "%s", PROMPT);
				rope_print(g_term.curr_buff->rope);
				//reprint_buffer(g_term.curr_buff);
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
