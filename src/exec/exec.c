/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/09 10:49:27 by alkozma           #+#    #+#             */
/*   Updated: 2019/11/21 13:27:26 by alkozma          ###   ########.fr       */
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
	//ft_printf("x\n");

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
	return (0);
}

int		find_exec(char *arg, char *envp, char **result)
{
	char			*line;
	char			*path;
	int				size;

	line = envp;
	ft_printf("jere\n");
	if (resolve_path(arg))
	{
		*result = ft_strdup(arg);
		return (1);
	}
	if (!line)
		return (0);
	while (*line)
	{
		size = ((ft_strchr(line, ':') < line) ? 0 :
			(ft_strchr(line, ':') - line));
		if (size)
		{
			path = ft_strsub(line, 0, size);
			line += line + size ? size + 1 : size;
			if (check_dir(arg, path))
			{
				*result = ft_makepath(path, arg, '/');
				free(envp);
				return (ft_returnfree(&path, 1));
			}
			free(path);
		}
		else
			break ;
	}
	return (ft_returnfree(&envp, 0));
}

int		get_env(char **envp, char *name, char **env_var)
{
	size_t			size;

	if (!name || !envp)
		return (0);
	if (!(find_env(name)))
		return (0);
	size = ft_strlen(name);
	while (*envp && ft_strncmp(*envp, name, size) != 0)
		envp++;
	if (*envp && ft_strncmp(*envp, name, size) != 0)
		return (0);
	*env_var = ft_strdup(*envp + size + 1);
	return (1);
}

int		check_path(char **name, char **args, char **envp)
{
	char			*env_var;
	int				res;

	env_var = NULL;
	res = get_env(envp, "PATH", &env_var);
	if (!(find_exec(*args, res ? ft_strjoin(env_var, ":") : NULL, name)))
		return (ft_returnfree(&env_var, 0));
	return (ft_returnfree(&env_var, 1));
}

int		readfd(int fd1, int fd2, int c2)
{
	char	buf[42];
	int		bytes;

	if (fd2 == -1)
		ft_printf_fd(STDERR_FILENO, "%s\n", strerror(errno));
	while ((bytes = read(fd1, &buf, 42)) > 0)
		write(fd2, &buf, bytes);
	close(c2 ? fd2 : fd1);
	return (1);
}
