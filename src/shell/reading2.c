/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reading2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/13 17:09:48 by calamber          #+#    #+#             */
/*   Updated: 2019/12/18 10:56:16 by calamber         ###   ########.fr       */
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
			ft_putstr(((start == end - 1 && !hd) || hd) ? "\e[94m" : "\e[32m");
			ft_putchar((start == end - 1 && !hd) ? '>' : pwd[start]);
			start++;
		}
		ft_putstr("\e[0m");
	}
}
