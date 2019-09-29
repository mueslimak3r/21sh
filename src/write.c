/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/20 00:37:55 by alkozma           #+#    #+#             */
/*   Updated: 2019/09/28 22:58:36 by alkozma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

int		term_write(char *str, int fd, int cmd)
{
	int		des;

	des = g_routes[fd] ? g_routes[fd] : fd;
	if (ft_strlen(str) < 1)
		return (1);
	if (!cmd)
	{
		
		if (g_term.conf.cursor[0] + ft_strlen(str) >= g_term.conf.termsize[0])
		{
			//if (g_term.conf.termsize[0] - g_term.conf.cursor[0] > 0)
			//	write(des, str, g_term.conf.termsize[0] - g_term.conf.cursor[0]);
			write(des, str, ft_strlen(str));
			ft_putstr_fd(" \n", STDERR_FILENO);
			//if (ft_strlen(str) - g_term.conf.termsize[0] + g_term.conf.cursor[0] > 0)
			//	write(des, str + g_term.conf.termsize[0] - g_term.conf.cursor[0], ft_strlen(str) - g_term.conf.termsize[0] + g_term.conf.cursor[0]);
			//g_term.conf.cursor[0] = ft_strlen(str) - g_term.conf.termsize[0] + g_term.conf.cursor[0];
			g_term.conf.cursor[0] = ft_strlen(str);
			g_term.conf.cursor[1]++;
		}
		else
		{
			g_term.conf.cursor[0] += ft_strlen(str);
			write(des, str, ft_strlen(str));
		}
		ft_putstr_fd("\u2588", STDERR_FILENO);
		ft_putstr_fd("\b", STDERR_FILENO);
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
	char buf[30];
	int	curpos;

	if (ft_strlen(str) < 1)
		return (1);
	ft_printf_fd(STDERR_FILENO, " ");
	ft_putstr_fd("\b", STDERR_FILENO);
	g_term.conf.cursor[0] -= 1;
	rem_from_buf(str, g_term.conf.cursor[0] - 2);
	if (g_term.conf.cursor[0] <= 0)
	{
		g_term.conf.cursor[0] = g_term.conf.termsize[0] - 1;
		g_term.conf.cursor[1]--;
		ft_printf_fd(STDERR_FILENO, "\033[1A");
		ft_printf_fd(STDERR_FILENO, "\033[%dC", g_term.conf.termsize[0] - 3);
		ft_printf_fd(STDERR_FILENO, "\u2588");
		ft_printf_fd(STDERR_FILENO, "\b");
	}
	else
	{
		ft_putstr_fd("\b", STDERR_FILENO);
		ft_putstr_fd("\u2588", STDERR_FILENO);
		ft_putstr_fd("\b", STDERR_FILENO);
	}
	return (1);
}

int		handle_controls(unsigned long code, char *str, char *saved)
{
	int ret = 0;

	if (code == DELETE)
		ret = delete_char(saved);
	else if (code == ENTER)
	{
		ft_printf_fd(STDERR_FILENO, " \b\n");
		//ft_putstr_fd("\n", STDERR_FILENO);
		g_term.conf.cursor[0] = ft_strlen(PROMPT);
		g_term.conf.cursor[1]++;
		ret = 1;
	}
	if (code == SPACE)
	{
		ft_printf_fd(STDERR_FILENO, "\nCURSOR POS: [%d,%d]\n", g_term.conf.cursor[0], g_term.conf.cursor[1]);
		ret = 1;
	}
	if (ret == 0)
		term_write(str, STDERR_FILENO, 0);
	//ft_printf_fd(STDERR_FILENO, " ");
	return (ret);
}
