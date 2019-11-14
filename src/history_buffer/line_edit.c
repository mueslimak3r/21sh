/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_edit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/13 22:37:43 by alkozma           #+#    #+#             */
/*   Updated: 2019/11/14 06:01:07 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

void        t_buff_line_rm(t_tbuff *buff, int pos, int size)
{
	char	*new;
	size_t	sz;

	if (pos < 0 || !buff || !buff->buff_str)
		return ;
	sz = ft_strlen(buff->buff_str) - size;
	if (!(new = ft_memalloc(sizeof(*new) * (sz + 1))))
        return ;
	ft_memcpy(new, buff->buff_str, pos);
    if (size < (int)ft_strlen(buff->buff_str) - pos)
	    ft_memcpy(new + pos, buff->buff_str + pos + size, ft_strlen(buff->buff_str) - pos - size);
	free(buff->buff_str);
	buff->buff_str = new;
	move_cursor(-size);
}

/*
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
*/
void        tbuff_line_insert(t_tbuff *buff, char *in, int pos)
{
    char    *tmp = NULL;
    int     new_size = 0;
    int     in_size = 0;

    if (!buff)
        return ;
    //ft_printf_fd(STDERR_FILENO, "in at %d\n", pos);
    in_size = ft_strlen(in);
    new_size = in_size + buff->len;
    if (!buff->buff_str || !*(buff->buff_str)) 
    {
        if (buff->buff_str && !*(buff->buff_str))
            ft_strdel(&(buff->buff_str));
        buff->buff_str = ft_strdup(in);
        buff->len = in_size;
        move_cursor(in_size);
        return ;
    }
    if (!(tmp = ft_strnew(new_size)))
        return ;
    if (pos == 0)
        ft_strcat_cpy(tmp, in, buff->buff_str, new_size);
    else if (pos >= buff->len)
        ft_strcat_cpy(tmp, buff->buff_str, in, new_size);
    else if (pos < buff->len)
    {
        ft_memmove(tmp, buff->buff_str, pos);
        ft_memmove(tmp + pos, in, in_size);
        ft_memmove(tmp + pos + in_size, buff->buff_str + pos, buff->len - pos);
    }
    if (tmp && *tmp)
    {
        ft_strdel(&(buff->buff_str));
        buff->buff_str = tmp;
        buff->len = new_size;
        move_cursor(in_size);
    }
    else if (tmp)
        ft_strdel(&tmp);
}