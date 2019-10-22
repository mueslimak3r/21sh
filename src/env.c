/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkozma <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/16 10:52:45 by alkozma           #+#    #+#             */
/*   Updated: 2019/10/20 05:21:29 by alkozma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

unsigned long	djb2(char *str)
{
	unsigned long	hash;
	int				c;

	hash = 5381;
	while ((c = *str++))
		hash = ((hash << 5) + hash) + c;
	return (hash);
}

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

int				ft_unsetenv(char *name)
{
	unsigned long	hash;
	t_ht			*tmp;

	hash = djb2(name);
	tmp = g_env[hash % HT_OVERHEAD];
	while (tmp && (unsigned long)(tmp->content_size) != hash)
		tmp = tmp->next;
	if (tmp)
	{
		free(tmp->content);
		free(tmp);
		tmp = NULL;
	}
	load_envp();
	return (1);
}

int				ft_setenv(char *name, char *val)
{
	unsigned long	hash;
	t_ht			*tmp;
	t_ht			*new;
	t_ht			*last;

	hash = djb2(name);
	tmp = g_env[hash % HT_OVERHEAD];
	new = malloc(sizeof(t_ht));
	new->content = ft_strdup(val);
	new->content_name = ft_strjoin(name, "=");
	new->content_size = hash;
	new->next = NULL;
	last = NULL;
	while (tmp)
	{
		if ((unsigned long)(tmp->content_size) == hash)
		{
			new->next = tmp->next;
			free(tmp->content);
			free(tmp->content_name);
			free(tmp);
			if (last)
				last->next = new;
			else
				g_env[hash % HT_OVERHEAD] = new;
			return (1);
		}
		last = tmp;
		tmp = tmp->next;
	}
	new->next = g_env[hash % HT_OVERHEAD];
	g_env[hash % HT_OVERHEAD] = new;
	load_envp();
	return (1);
}

int				ft_export(char *str)
{
	char	**split;

	if (!str)
		return (0);
	split = ft_strsplit(str, '=');
	if (!split[0] || !split[1])
		return (0);
	ft_setenv(split[0], split[1]);
	free(split[0]);
	free(split[1]);
	free(split);
	split = NULL;
	return (1);
}

/*
** THERE IS A BETTER WAY TO DO THIS
** MAKE AN ARRAY OF CREATED HASHES, ITERATE THROUGH THAT LIST
** INSTEAD OF ITERATING THROUGH ALL ELEMENTS IN HASH TABLE
*/

int				load_envp(void)
{
	int		i;
	int		b;
	char	*new[HT_OVERHEAD];
	t_env	ret;

	i = 0;
	b = 0;
	while (i < HT_OVERHEAD)
	{
		if (g_env[i])
			new[b++] = ft_strjoin(g_env[i]->content_name, g_env[i]->content);
		i++;
	}
	new[b] = 0;
	ret.envp = new;
	ret.size = b;
	g_term.env = ret;
	return (1);
}

int				init_env(void)
{
	extern char	**environ;
	int			i;
	char		**split;

	i = 0;
	ft_bzero(g_env, sizeof(t_ht*) * HT_OVERHEAD);
	while (environ[i])
	{
		ft_export(environ[i]);
		i++;
	}
	return (1);
}

int			ft_env(char **envp)
{
	while (*envp)
		ft_printf_fd(STDERR_FILENO, "%s\n", *(envp++));
	return (0);
}

int			run_builtins(char **args, t_env *env)
{
	if (ft_strcmp(args[0], "exit") == 0)
		exit(0);
	else if (ft_strcmp(args[0], "env") == 0)
		return (ft_env(env->envp));
	else if (ft_strcmp(args[0], "export") == 0)
		return (ft_export(args[1]));
	else if (ft_strcmp(args[0], "alias") == 0)
		return (ft_alias(args[1]));
	else if (ft_strcmp(args[0], "cd") == 0)
		return(ft_cd(args[1]));
	return (2);
}
