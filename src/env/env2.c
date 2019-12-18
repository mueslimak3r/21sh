/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/25 03:17:44 by calamber          #+#    #+#             */
/*   Updated: 2019/12/17 16:24:40 by alkozma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

char			*find_env(char *name)
{
	t_ht			*tmp;
	unsigned long	hash;

	hash = djb2(name);
	tmp = g_env[hash % HT_OVERHEAD];
	while (tmp && (unsigned long)(tmp->content_size) != hash)
		tmp = tmp->next;
	return (tmp ? (char*)(tmp->content) : NULL);
}

static int		reset_return(unsigned long hash)
{
	if (g_env[hash % HT_OVERHEAD])
	{
		free(g_env[hash % HT_OVERHEAD]->content);
		free(g_env[hash % HT_OVERHEAD]->content_name);
		free(g_env[hash % HT_OVERHEAD]);
		g_env[hash % HT_OVERHEAD] = NULL;
		load_envp();
	}
	return (1);
}

int				ft_unsetenv(char *name)
{
	unsigned long	hash;
	t_ht			*tmp;
	t_ht			*prev;

	hash = djb2(name);
	tmp = g_env[hash % HT_OVERHEAD];
	if (!tmp || !tmp->next)
		return (reset_return(hash));
	prev = NULL;
	while (tmp && (unsigned long)(tmp->content_size) != hash)
	{
		prev = tmp;
		tmp = tmp->next;
	}
	if (tmp)
	{
		free(tmp->content);
		free(tmp->content_name);
		if (prev)
			prev->next = tmp->next ? tmp->next : NULL;
		free(tmp);
		tmp = NULL;
	}
	load_envp();
	return (1);
}

int				setenv_helper(unsigned long hash, t_ht *new)
{
	t_ht	*last;
	t_ht	*tmp;

	last = NULL;
	tmp = g_env[hash % HT_OVERHEAD];
	while (tmp)
	{
		if ((unsigned long)(tmp->content_size) == hash)
		{
			new->next = tmp->next;
			free(tmp->content);
			free(tmp->content_name);
			tmp->content = new->content;
			tmp->content_name = new->content_name;
			free(new);
			load_envp();
			return (1);
		}
		last = tmp;
		tmp = tmp->next;
	}
	return (0);
}

int				ft_setenv(char *name, char *val)
{
	unsigned long	hash;
	t_ht			*new;

	hash = djb2(name);
	new = malloc(sizeof(t_ht));
	new->content = ft_strdup(val);
	new->content_name = ft_strjoin(name, "=");
	new->content_size = hash;
	new->next = NULL;
	if (setenv_helper(hash, new))
		return (1);
	new->next = g_env[hash % HT_OVERHEAD];
	g_env[hash % HT_OVERHEAD] = new;
	load_envp();
	return (1);
}
