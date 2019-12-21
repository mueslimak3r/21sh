/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alias.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/16 14:23:18 by alkozma           #+#    #+#             */
/*   Updated: 2019/12/20 15:59:13 by alkozma          ###   ########.fr       */
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
		{
			free(name);
			return (ft_strdup((char*)(tmp->content)));
		}
		tmp = tmp->next;
	}
	return (name);
}

int		insert_node(t_ht *new, unsigned long hash, char **split)
{
	t_ht	*tmp;

	tmp = g_alias[hash % HT_OVERHEAD];
	while (tmp)
	{
		if ((unsigned long)(tmp->content_size) == hash)
		{
			free(tmp->content_name);
			free(tmp->content);
			tmp->content = ft_strdup(split[1]);
			tmp->content_name = ft_strdup(split[0]);
			free(new->content);
			free(new->content_name);
			free(new);
			return (1);
		}
		if (!tmp->next)
		{
			tmp->next = new;
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

char	**check_split(char *str)
{
	char	**split;

	split = ft_strsplit(str, '=');
	if (!split[0] || !split[1])
	{
		if (split[0])
		{
			free(split[0]);
			split[0] = NULL;
		}
		if (split)
			free(split);
		return (NULL);
	}
	return (split);
}

int		ft_alias(char *str)
{
	unsigned long	hash;
	t_ht			*new;
	char			**split;
	int				i;

	split = NULL;
	if (!str || !(split = check_split(str)))
		return (0);
	hash = djb2(split[0]);
	new = malloc(sizeof(t_ht));
	new->content_size = hash;
	new->next = NULL;
	new->content_name = ft_strdup(split[0]);
	new->content = ft_strdup(split[1]);
	if (insert_node(new, hash, split))
		return (free_arr(split));
	i = 0;
	while (split[i])
	{
		free(split[i]);
		split[i++] = NULL;
	}
	free(split);
	g_alias[hash % HT_OVERHEAD] = new;
	return (1);
}
