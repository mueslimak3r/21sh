/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkozma <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/20 05:01:29 by alkozma           #+#    #+#             */
/*   Updated: 2019/10/20 05:30:55 by alkozma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

char	*path_expansions(char *path)
{
	char	*ret;

	ret = NULL;
	if (path[0] == '~')
		ret = ft_strjoin(find_env("HOME"), path + 1);
	else if (path[0] == '-' && !path[1])
		ret = ft_strdup(find_env("OLDPWD") ? find_env("OLDPWD") : find_env("PWD"));
	else if (path[0] != '/')
	{
		ret = ft_strjoin("/", path);
		ret = ft_strjoin(find_env("PWD"), ret);
	}
	if (!ret)
		return (ft_strdup(path));
	return (ret);
}

int		ft_cd(char *path)
{
	struct stat	stats;
	char		*tmp;

	tmp = path_expansions(path);
	if (stat(tmp, &stats) == 0 && S_ISDIR(stats.st_mode))
	{
		ft_setenv("OLDPWD", find_env("PWD"));
		ft_setenv("PWD", tmp);
		chdir(tmp);
	}
	else
	{
		free(tmp);
		return (-1);
	}
	load_envp();
	free(tmp);
	return (1);
}
