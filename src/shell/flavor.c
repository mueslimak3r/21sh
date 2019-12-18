/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flavor.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/11 17:57:05 by alkozma           #+#    #+#             */
/*   Updated: 2019/12/18 09:01:12 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

void	choose_color(int pos, int fd)
{
	static char *colors[6] = {
		"\e[31m", "\e[91m", "\e[93m",
		"\e[32m", "\e[96m",
		"\e[95m"
	};

	ft_putstr_fd(colors[pos % 5], fd);
}

int		rainbow_it(char *s, int pos, int fd)
{
	int i;

	i = 0;
	while (s[i])
	{
		pos++;
		choose_color(pos, fd);
		ft_putchar_fd(s[i], fd);
		i++;
	}
	return (pos);
}

void	print_banner(int fd)
{
	char	buf[42];
	int		file;
	int		read_bytes;
	int		pos;

	pos = 0;
	file = open("banner", O_RDONLY);
	while ((read_bytes = read(file, &buf, 41)) >= 0)
	{
		buf[read_bytes] = 0;
		pos = rainbow_it(buf, pos, fd);
		if (read_bytes == 0)
			break ;
	}
	ft_putstr("\e[0m");
	close(file);
}
