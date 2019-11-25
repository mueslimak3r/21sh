/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/20 05:01:29 by alkozma           #+#    #+#             */
/*   Updated: 2019/11/22 21:36:52 by alkozma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

char	*path_expansions(char *path)
{
	char	*ret;
	char	*tmp;

	ret = NULL;
	tmp = NULL;
	if (!path)
		return (NULL);
	else if (path[0] == '-' && !path[1])
		ret = ft_strdup(find_env("OLDPWD") ?
			find_env("OLDPWD") : find_env("PWD"));
	else if (path[0] != '/')
	{
		tmp = ft_strjoin("/", path);
		ret = ft_strjoin(find_env("PWD"), tmp);
		free(tmp);
		tmp = NULL;
	}
	if (!ret)
		return (ft_strdup(path));
	return (ret);
}

int		ft_cd(char *path)
{
	struct stat	stats;
	char		*tmp;

	if (!(tmp = path_expansions(path)))
		tmp = ft_strdup(find_env("HOME"));
	if (stat(tmp, &stats) == 0 && S_ISDIR(stats.st_mode))
	{
		ft_setenv("OLDPWD", find_env("PWD"));
		ft_setenv("PWD", tmp);
		chdir(tmp);
	}
	else
	{
		ft_printf_fd(STDERR_FILENO, "-wtsh: cd: %s: No such file or directory\n", tmp);
		free(tmp);
		return (-1);
	}
	load_envp();
	free(tmp);
	return (1);
}
