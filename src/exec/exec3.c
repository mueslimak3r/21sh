/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkozma <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 21:34:26 by alkozma           #+#    #+#             */
/*   Updated: 2019/11/21 21:40:34 by alkozma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

int		check_dir(char *name, char *path)
{
	struct dirent	*pdirent;
	DIR				*pdir;

	pdir = opendir(path);
	if (!pdir)
		return (0);
	while ((pdirent = readdir(pdir)) != 0)
	{
		if (ft_strcmp(pdirent->d_name, name) == 0)
		{
			closedir(pdir);
			return (1);
		}
	}
	closedir(pdir);
	return (0);
}

int		resolve_path(char *arg)
{
	int		pos;
	char	*path;
	char	*name;

	path = NULL;
	name = NULL;
	if ((pos = last_slash(arg)) < 0)
		return (0);
	path = ft_strsub(arg, 0, pos + 1);
	name = ft_strsub(arg, pos + 1, ft_strlen(arg));
	if (check_dir(name, path))
	{
		free(path);
		free(name);
		return (1);
	}
	free(path);
	free(name);
	return (0);
}
