/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_buffer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/25 03:27:26 by calamber          #+#    #+#             */
/*   Updated: 2019/10/25 07:38:09 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

void		tbuff_push(t_tbuff **buff, char *s)
{
	t_tbuff *new;

	if (!s || !*s)
		return ;
	while (*s == 27)
		s++;
	if (!*s)
		return ;
	if (!(new = ft_memalloc(sizeof(t_tbuff))))
		return ;
	if (*buff)
	{
		new->prev = (*buff)->prev;
		if ((*buff)->prev)
			(*buff)->prev->next = new;
		(*buff)->prev = new;
		new->next = *buff;
	}
	else
	{
		new->next = NULL;
		new->prev = NULL;
	}
	new->s = ft_strdup(s);
	new->size = ft_strlen(s);
	*buff = new;
}

char		*tbuff_peek(t_tbuff *buff)
{
	if (!buff || !(buff->s))
		return (NULL);
	return (ft_strdup(buff->s));
}

void		tbuff_free(t_tbuff **buff)
{
	t_tbuff *tmp;

	while (*buff)
	{
		tmp = *buff;
		*buff = (*buff)->next;
		if (tmp == *buff)
			*buff = NULL;
		free(tmp->s);
		free(tmp);
	}
	*buff = NULL;
}

void		tbuff_print(t_tbuff *buff)
{
	ft_printf_fd(STDERR_FILENO, "start of buff\n");
	while (buff)
	{
		ft_printf_fd(STDERR_FILENO, "%s\n", tbuff_peek(buff));
		buff = buff->next;
	}
	ft_printf_fd(STDERR_FILENO, "end of buff\n");
}