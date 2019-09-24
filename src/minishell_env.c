/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/24 10:07:26 by calamber          #+#    #+#             */
/*   Updated: 2019/09/24 06:21:29 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

int			find_env(char **envp, char *name)
{
	size_t	size;
	int		i;

	i = 0;
	if (!name || !envp)
		return (0);
	size = ft_strlen(name);
	while (envp[i] && ft_strncmp(envp[i], name, size) != 0)
		i++;
	if (envp[i] && ft_strncmp(envp[i], name, size) == 0)
		return (i);
	return (-1);
}

int			ft_unsetenv(char *name, t_env *env)
{
	char	**new;
	int		pos;
	int		i;

	i = 0;
	if (!name)
		return (-1);
	if ((pos = find_env(env->envp, name)) == -1)
		return (-1);
	if (!(new = ft_memalloc(sizeof(char*) * (--(env->size) + 1))))
		return (-1);
	while (i < env->size)
	{
		if (env->envp[i < pos ? i : i + 1])
			new[i] = ft_strdup(env->envp[i < pos ? i : i + 1]);
		i++;
	}
	ft_arraydel(env->envp);
	env->envp = new;
	return (0);
}

int			ft_setenv(char *name, char *val, t_env *env)
{
	int		i;
	char	**new;

	i = 0;
	if (!name || !val)
		return (-1);
	if ((find_env(env->envp, name)) != -1)
		return (-1);
	if (!(new = ft_memalloc(sizeof(char*) * (++(env->size) + 1))))
		return (-1);
	while (i < env->size - 1)
	{
		new[i] = ft_strdup(env->envp[i]);
		i++;
	}
	new[i] = ft_makepath(name, val, '=');
	ft_arraydel(env->envp);
	env->envp = new;
	return (0);
}

int			ft_env(char **envp)
{
	while (*envp)
		ft_printf("%s\n", *(envp++));
	return (0);
}

int			run_builtins(char **args, t_env *env)
{
	if (ft_strcmp(args[0], "exit") == 0)
		return (1);
	else if (ft_strcmp(args[0], "env") == 0)
		return (ft_env(env->envp));
	else if (ft_strcmp(args[0], "setenv") == 0)
		return (ft_setenv(args[1] ? args[1] : 0, (args[2] ? args[2] : 0), env));
	else if (ft_strcmp(args[0], "unsetenv") == 0)
		return (ft_unsetenv(args[1], env));
	return (2);
}
