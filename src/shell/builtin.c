/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/20 05:01:29 by alkozma           #+#    #+#             */
/*   Updated: 2019/12/20 17:48:55 by alkozma          ###   ########.fr       */
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
	char		buf[255 + 1];
	int			ret;

	ret = -1;
	if (!(tmp = path_expansions(path)))
		tmp = find_env("HOME") ? ft_strdup(find_env("HOME")) : NULL;
	if (tmp && !access(tmp, X_OK) && stat(tmp, &stats) == 0 &&
			S_ISDIR(stats.st_mode) && (ret = 1))
	{
		ft_setenv("OLDPWD", find_env("PWD"));
		chdir(tmp);
		ft_memset(buf, 0, 256);
		getcwd(buf, 255);
		ft_setenv("PWD", buf);
	}
	else if (!tmp)
		ft_printf_fd(STDERR_FILENO, "-wtsh: cd: home not set\n");
	else
		ft_printf_fd(STDERR_FILENO, "-wtsh: cd: %s: %s\n", tmp ?
				tmp : path, strerror(errno));
	load_envp();
	free(tmp);
	return (ret);
}
