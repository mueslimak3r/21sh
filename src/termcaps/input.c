/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/20 00:37:55 by alkozma           #+#    #+#             */
/*   Updated: 2019/11/13 22:24:24 by alkozma          ###   ########.fr       */
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
		tputs(tgetstr("do", NULL), 0, ft_charput);
        g_term.conf.cursor[0] = amt;
        g_term.conf.cursor[1]++;
        g_term.conf.curlines++;
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
		move_cursor(-1);
		reprint_buffer(g_term.curr_buff);
	}
	else if (code == ENTER)
	{
		ft_printf_fd(STDERR_FILENO, " \b\n");
		//
		// set cursor to end of buff line
		//
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
		int len = ft_strlen(g_term.curr_buff->buff_str);
		if (code == UP)
		{
			//ft_printf_fd(STDERR_FILENO, "up\n");
			if (g_term.curr_buff && g_term.curr_buff->next)
			{
				g_term.curr_buff = g_term.curr_buff->next;
				while (g_term.conf.curlines)
				{
					tputs(tgetstr("cr", NULL), 0, ft_charput);
					tputs(tgetstr("cd", NULL), 0, ft_charput);
					g_term.conf.curlines--;
				}
				g_term.conf.curlines = 1;

				//
				// get len of buff_str
				//

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

				// reprint buffer

			}
		}
		if (code == DOWN)
		{
			if (g_term.curr_buff && g_term.curr_buff->prev)
			{
				g_term.curr_buff = g_term.curr_buff->prev;
				while (g_term.conf.curlines)
				{
					tputs(tgetstr("cr", NULL), 0, ft_charput);
					tputs(tgetstr("cd", NULL), 0, ft_charput);
					g_term.conf.curlines--;
				}
				g_term.conf.curlines = 1;
				
				//
				// get len of buff_str
				//
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

				// reprint buffer

			}
		}
		if (code == LEFT || code == RIGHT)
		{
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
