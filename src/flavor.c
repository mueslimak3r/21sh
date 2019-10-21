/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flavor.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkozma <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/11 17:57:05 by alkozma           #+#    #+#             */
/*   Updated: 2019/10/16 13:23:23 by alkozma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

void	print_banner(int fd)
{
	char	buf[42];
	int		file;
	int		read_bytes;

	file = open("banner", O_RDONLY);
	while ((read_bytes = read(file, &buf, 41)) >= 0)
	{
		buf[read_bytes] = 0;
		ft_printf_fd(fd, "%s", buf);
		if (read_bytes == 0)
			break ;
	}
	close(file);
}
