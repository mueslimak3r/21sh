/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkozma <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/20 00:37:55 by alkozma           #+#    #+#             */
/*   Updated: 2019/09/20 03:09:57 by alkozma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

int		term_write(char *str, int fd)
{
	int		des;

	des = g_routes[fd] ? g_routes[fd] : fd;
	write(des, str, ft_strlen(str));
	return (1);
}

int		delete_char(char *str)
{
	term_write("\b", STDERR_FILENO);
	term_write(" ", STDERR_FILENO);
	term_write("\b", STDERR_FILENO);
	str[ft_strlen(str) - 1] = 0;
	return (1);
}

int		handle_controls(unsigned long code, char *str)
{
	if (code == DELETE)
		return (delete_char(str));
	return (0);
}
