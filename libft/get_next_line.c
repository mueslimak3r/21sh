/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/30 19:38:09 by calamber          #+#    #+#             */
/*   Updated: 2019/03/24 10:20:15 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#define RET_IF(cond, ret) if (cond) return (ret)

static void			gnl_loop(int fd, char **line, char buf[BUFF_SIZE], int nl)
{
	ssize_t			rret;
	char			*tmp1;
	char			*tmp2;
	size_t			len;

	rret = 1;
	while (!(nl) && rret > 0)
	{
		rret = read(fd, buf, BUFF_SIZE);
		len = 0;
		while (len < BUFF_SIZE && buf[len] != '\n')
			len++;
		if (buf[len] == '\n')
			nl = 1;
		tmp2 = ft_strndup(buf, len);
		tmp1 = ft_strjoin(*line, tmp2);
		free(*line);
		*line = tmp1;
		free(tmp2);
		ft_bzero(buf, len + nl);
	}
}

static size_t		gnl_find_i(char buf[BUFF_SIZE], size_t *i)
{
	*i = 0;
	while (*i < BUFF_SIZE && buf[*i] == 0)
		*i = *i + 1;
	if (*i == BUFF_SIZE)
	{
		*i = 0;
		return (1);
	}
	return (0);
}

static size_t		gnl_find_len(char buf[BUFF_SIZE], size_t i)
{
	size_t len;

	len = 0;
	while (i + len < BUFF_SIZE && buf[i + len] != '\n')
		len++;
	return (len);
}

int					get_next_line(const int fd, char **line)
{
	static char		buf[255][BUFF_SIZE];
	size_t			i;
	size_t			len;
	ssize_t			rret;
	int				nl;

	RET_IF(fd < 0 || fd >= 255 || !(line) || BUFF_SIZE < 1, -1);
	rret = BUFF_SIZE;
	nl = 0;
	if (gnl_find_i(buf[fd], &i))
	{
		RET_IF((rret = read(fd, buf[fd], BUFF_SIZE)) == -1, -1);
		RET_IF(rret == 0, 0);
	}
	len = gnl_find_len(buf[fd], i);
	*line = ft_strndup(buf[fd] + i, len);
	if (buf[fd][i + len] == '\n')
	{
		len++;
		nl = 1;
	}
	ft_bzero(buf[fd] + i, len);
	gnl_loop(fd, line, buf[fd], nl);
	return (1);
}
