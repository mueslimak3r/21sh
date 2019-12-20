/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_edit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/13 22:37:43 by alkozma           #+#    #+#             */
/*   Updated: 2019/12/20 10:25:43 by alkozma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

void		t_buff_line_rm(t_tbuff *buff, int pos, int size)
{
	char	*new;
	size_t	sz;

	if (pos < 0 || !buff || !buff->buff_str)
		return ;
	sz = ft_strlen(buff->buff_str) - size;
	if (sz <= 0)
	{
		ft_strdel(&buff->buff_str);
		buff->len = 0;
		return ;
	}
	if (!(new = ft_memalloc(sizeof(*new) * (sz + 1))))
		return ;
	ft_memcpy(new, buff->buff_str, pos);
	if (size < (int)ft_strlen(buff->buff_str) - pos)
		ft_memcpy(new + pos, buff->buff_str + pos + size,
				ft_strlen(buff->buff_str) - pos - size);
	free(buff->buff_str);
	buff->buff_str = new;
	buff->len = sz;
}

void		tbuff_insert_helper(t_tbuff **buff, char *in, int pos, int in_size)
{
	char	*tmp;
	int		new_size;

	new_size = in_size + (*buff)->len;
	if (!(tmp = ft_strnew(new_size)))
		return ;
	if (pos == 0)
		ft_strcat_cpy(tmp, in, (*buff)->buff_str, new_size);
	else if (pos >= (*buff)->len)
		ft_strcat_cpy(tmp, (*buff)->buff_str, in, new_size);
	else if (pos < (*buff)->len)
	{
		ft_memmove(tmp, (*buff)->buff_str, pos);
		ft_memmove(tmp + pos, in, in_size);
		ft_memmove(tmp + pos + in_size, (*buff)->buff_str + pos,
											(*buff)->len - pos);
	}
	ft_strdel(&((*buff)->buff_str));
	(*buff)->buff_str = tmp;
	(*buff)->len = new_size;
}

static int	count_char(char *data, char c)
{
	int	i;
	int ret;

	i = 0;
	ret = 0;
	while (data && data[i])
	{
		if (data[i] == c)
			ret++;
		i++;
	}
	return (ret);
}

char		*convert_tabs_spaces(char *data)
{
	char	*ret;
	int		a;
	int		b;
	int		c;

	ret = ft_memalloc(ft_strlen(data) + (count_char(data, '\t') * 3) + 1);
	a = 0;
	b = 0;
	while (data[a])
	{
		if (data[a] == '\t')
		{
			c = 4;
			while (c--)
				ret[b++] = ' ';
			a++;
		}
		else
			ret[b++] = data[a++];
	}
	return (ret);
}

void		tbuff_line_insert(t_tbuff **buff, char *in, int pos)
{
	int		in_size;

	if (!buff)
		return ;
	in_size = ft_strlen(in);
	if (buff && !(*buff)->temp)
		tbuff_replicate(buff);
	if (!(*buff)->buff_str || !(*(*buff)->buff_str))
	{
		if ((*buff)->buff_str && !*((*buff)->buff_str))
			ft_strdel(&((*buff)->buff_str));
		(*buff)->buff_str = ft_strdup(in);
		(*buff)->len = in_size;
		return ;
	}
	tbuff_insert_helper(buff, in, pos, in_size);
}
