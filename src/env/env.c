/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/16 10:52:45 by alkozma           #+#    #+#             */
/*   Updated: 2019/12/20 16:06:39 by alkozma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

int				ft_export(char *str)
{
	char	**split;
	int		i;

	if (!str)
		return (0);
	split = ft_strsplit(str, '=');
	if (!split[0] || !split[1])
	{
		if (split[0])
			free(split[0]);
		free(split);
		return (0);
	}
	ft_setenv(split[0], split[1]);
	i = 0;
	while (split[i])
	{
		free(split[i]);
		split[i++] = NULL;
	}
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

	i = 0;
	b = 0;
	while (i < g_term.env.size && g_term.env.envp[i])
	{
		free(g_term.env.envp[i]);
		g_term.env.envp[i++] = NULL;
	}
	i = 0;
	while (i < HT_OVERHEAD)
	{
		if (g_env[i])
		{
			g_term.env.envp[b++] =
				ft_strjoin(g_env[i]->content_name,
						g_env[i]->content);
		}
		i++;
	}
	g_term.env.envp[b] = 0;
	g_term.env.size = b;
	return (1);
}

int				init_env(void)
{
	extern char	**environ;
	int			i;
	char		*tmp;

	i = 0;
	ft_bzero(g_env, sizeof(t_ht*) * HT_OVERHEAD);
	while (environ[i])
	{
		ft_export(environ[i]);
		i++;
	}
	tmp = ft_strjoin_free(
		"SHLVL=", ft_itoa((find_env("SHLVL")
		? ft_atoi(find_env("SHLVL")) : 0) + 1), 1);
	ft_export(tmp);
	free(tmp);
	return (1);
}

int				ft_env(char **envp)
{
	while (*envp)
		ft_printf_fd(STDERR_FILENO, "%s\n", *(envp++));
	return (0);
}

void			add_dir_to_st(char *path, struct s_st **tree)
{
	DIR		*pdir;
	struct dirent	*pdirent;

	pdir = opendir(path);
	if (!pdir)
		return ;
	while ((pdirent = readdir(pdir)) != 0)
	{
		if (ft_strcmp(path, "/") == 0)
			st_insert_str(ft_strjoin(path, pdirent->d_name), tree);
		else
			st_insert_str(ft_strjoin_free(ft_strjoin(path, "/"), pdirent->d_name, 0), tree);
	}
	closedir(pdir);
}

int				run_builtins(char **args, t_env *env)
{
	if (ft_strcmp(args[0], "exit") == 0)
	{
		reset_term();
		exit(args[1] ? ft_atoi(args[1]) : 0);
	}
	else if (ft_strcmp(args[0], "env") == 0)
		return (ft_env(env->envp));
	else if (ft_strcmp(args[0], "export") == 0)
		return (ft_export(args[1]));
	else if (ft_strcmp(args[0], "unset") == 0)
		return (ft_unsetenv(args[1]));
	else if (ft_strcmp(args[0], "alias") == 0 && args[1])
		return (ft_alias(args[1]));
	else if (ft_strcmp(args[0], "cd") == 0)
		return (ft_cd(args[1]));
	else if (ft_strcmp(args[0], "alias") == 0 && args[0] && !args[1])
		return (print_alias());
	else if (ft_strcmp(args[0], "match") == 0 && args[1])
	{
		struct s_st	*cont;

		cont = NULL;
		add_dir_to_st(ft_strsub(args[1], 0, last_slash(args[1]) ? last_slash(args[1]) : 1), &cont);
		ft_printf_fd(STDERR_FILENO, "\n[%s]\n", st_match(args[1], cont));
		st_clean(cont);
		return (1);
	}
	return (2);
}
