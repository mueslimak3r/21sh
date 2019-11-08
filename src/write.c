/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/20 00:37:55 by alkozma           #+#    #+#             */
/*   Updated: 2019/11/07 21:44:26 by calamber         ###   ########.fr       */
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

int		redo_buffer(char *new_buffer)
{
	while ((g_term.conf.curlines--))
		ft_printf_fd(STDERR_FILENO, "\33[2K\r%s", g_term.conf.curlines ? "\33[1A" : "");
	g_term.conf.curlines = 1;
	g_term.conf.cursor[0] = 2;
	g_term.conf.cursor[1] = 0;
	if (g_term.line_in)
		free(g_term.line_in);
	g_term.line_in = ft_strdup(new_buffer);
	ft_printf_fd(STDERR_FILENO, "%s", PROMPT);
	term_write(new_buffer, STDERR_FILENO, 1);
	return (0);
}

int		ft_charput(int c)
{
	return (write(1, &c, 1));
}

int		reprint_buffer(t_tbuff *buff, int i)
{
	tputs(tgetstr("dl", NULL), 0, ft_charput);
	tputs(tgetstr("cr", NULL), 0, ft_charput);
	while ((g_term.conf.curlines))
	{
		g_term.conf.curlines--;
	}	//ft_printf_fd(STDERR_FILENO, "\33[2K\r%s", g_term.conf.curlines ? "\33[1A" : "");
	g_term.conf.curlines = 1;
	g_term.conf.cursor[0] = 2;
	g_term.conf.cursor[1] = 0;
	ft_printf_fd(STDERR_FILENO, "%s", PROMPT);
	if (buff)
	{
		if (buff->rope_buff[0] && i >= buff->cursor - 1)
			term_write(buff->rope_buff, STDERR_FILENO, 1);
		rope_print_from_index(buff->rope, 1);
		//if (buff->rope)
		//	rope_print(buff->rope);
		if (buff->rope_buff[0] && i < buff->cursor - 1)
			term_write(buff->rope_buff, STDERR_FILENO, 1);
	}
	return (0);
}

int		handle_controls(unsigned long code, char *str, char *saved)
{
	int ret = 0;

	if (code == DELETE)
		ret = delete_char(saved);
	else if (code == ENTER)
	{
		ft_printf_fd(STDERR_FILENO, " \b\n");
		g_term.conf.cursor[0] = ft_strlen(PROMPT);
		g_term.conf.cursor[1]++;
	}
	else if (code == TAB)
	{
		auto_complete();
	}
	else if (code == UP || code == DOWN || code == LEFT || code == RIGHT)
	{
		tbuff_move_cursor(g_term.curr_buff, code, str);
	}
	else
		ret -= 1;
	ret += 1;
	//if (ret == 0)
	//	term_write(str, STDERR_FILENO, 0);
	//else
	/*
	if (ret == 0)
	{
		term_write(str, STDERR_FILENO, 0);
	*/
	if (ret != 0)
		ft_memset(str, 0, BUFF_SIZE + 1);
	//ft_printf_fd(STDERR_FILENO, "code: %lu\n", code);
	return (ret);
}
