/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alias.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkozma <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/16 14:23:18 by alkozma           #+#    #+#             */
/*   Updated: 2019/10/16 15:09:08 by alkozma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

char	*find_alias(char *name)
{
	unsigned long	hash;
	t_ht			*tmp;

	hash = djb2(name);
	tmp = g_alias[hash % HT_OVERHEAD];
	while (tmp)
	{
		if ((unsigned long)(tmp->content_size) == hash)
			return ((char*)(tmp->content));
		tmp = tmp->next;
	}
	return (name);
}

int		ft_alias(char *str)
{
	unsigned long	hash;
	t_ht			*tmp;
	t_ht			*new;
	char			**split;

	if (!str)
		return (0);
	split = ft_strsplit(str, '=');
	if (!split[0] || !split[1])
		return (0);
	hash = djb2(split[0]);
	tmp = g_alias[hash % HT_OVERHEAD];
	new = malloc(sizeof(t_ht));
	new->content_size = hash;
	new->next = NULL;
	new->content = ft_strdup(split[1]);
	new->content_name = ft_strdup(split[0]);
	ft_printf_fd(STDERR_FILENO, "ALIASING %s -> %s\n", split[0], split[1]);
	while (tmp)
	{
		if ((unsigned long)(tmp->content_size) == hash)
		{
			free(tmp->content_name);
			free(tmp->content);
			tmp->content = ft_strdup(split[1]);
			tmp->content_name = ft_strdup(split[0]);
			return (1);
		}
		if (!tmp->next)
		{
			tmp->next = new;
			return (1);
		}
		tmp = tmp->next;
	}
	g_alias[hash % HT_OVERHEAD] = new;
	return (1);
}