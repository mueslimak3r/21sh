/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkozma <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/20 00:37:55 by alkozma           #+#    #+#             */
/*   Updated: 2019/09/21 14:47:56 by alkozma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

int		term_write(char *str, int fd, t_shellconf *conf)
{
	int		des;

	des = g_routes[fd] ? g_routes[fd] : fd;
	write(des, str, ft_strlen(str));
	conf->cursor[0] += ft_strlen(str);
	if (conf->cursor[0] >= conf->termsize[0])
	{
		ft_putstr_fd("\n", STDERR_FILENO);
		conf->cursor[0] -= (conf->termsize[0] + 1);
		conf->cursor[1]++;
	}
	return (1);
}

int		delete_char(char *str, t_shellconf *conf)
{
	if (!conf)
		return (-1);
	conf->cursor[0]--;
	if (conf->cursor[0] < 0)
	{
		conf->cursor[0] = conf->termsize[0] - 2;
		conf->cursor[1]--;
		ft_printf_fd(STDERR_FILENO, "\033[1A");
		ft_printf_fd(STDERR_FILENO, "\033[%dC", conf->termsize[0]);
		ft_printf_fd(STDERR_FILENO, "\033[s");
		ft_printf_fd(STDERR_FILENO, " ");
		ft_printf_fd(STDERR_FILENO, "\033[u");
	}
	else
	{
		term_write("\b", STDERR_FILENO, conf);
		term_write(" ", STDERR_FILENO, conf);
		term_write("\b", STDERR_FILENO, conf);
	}
	str[ft_strlen(str) - 1] = 0;
	return (1);
}

int		handle_controls(unsigned long code, char *str, t_shellconf *conf)
{
	if (code == DELETE)
		return (delete_char(str, conf));
	if (code == SPACE)
		ft_printf_fd(STDERR_FILENO, "\nCURSOR POS: [%d,%d]\n", conf->cursor[0], conf->cursor[1]);
	return (0);
}
