/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_edit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkozma <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/13 22:37:43 by alkozma           #+#    #+#             */
/*   Updated: 2019/11/13 22:55:54 by alkozma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

void        t_buff_line_rm(t_tbuff *buff, int pos, int size)
{
	char	*new;
	size_t	sz;

	if (!buff || !buff->buff_str)
		return ;
	sz = ft_strlen(buff->buff_str) - size;
	new = malloc(sizeof(char) * (sz + 1));
	ft_memcpy(new, buff->buff_str, pos);
	ft_memcpy(new, buff->buff_str + pos + size, sz - pos - size);
	free(buff->buff_str);
	buff->buff_str = new;
	move_cursor(-size);
}

void        tbuff_line_insert(t_tbuff *buff, char *in, int pos)
{
	char	*new;
	size_t	sz;

	if (!buff || !buff->buff_str)
	{
		buff->buff_str = ft_strdup(in);
		return ;
	}
	sz = ft_strlen(buff->buff_str) + ft_strlen(in);
	new = malloc(sizeof(char) * (sz + 1));
	ft_memcpy(new, buff->buff_str, pos);
	ft_memcpy(new + pos, in, ft_strlen(in));
	ft_memcpy(new + pos + ft_strlen(in), buff->buff_str + pos, sz - pos);
	new[sz] = 0;
	free(buff->buff_str);
	buff->buff_str = new;
	move_cursor(ft_strlen(in));
}
