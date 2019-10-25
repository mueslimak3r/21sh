/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/09 10:49:27 by alkozma           #+#    #+#             */
/*   Updated: 2019/10/25 03:24:15 by calamber         ###   ########.fr       */
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

int		find_exec(char *arg, char *envp, char **result)
{
	char			*line;
	char			*path;
	int				size;

	line = envp;
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

	env_var = 0;
	if (!get_env(envp, "PATH", &env_var))
		return (0);
	if (!(find_exec(*args, ft_strjoin(env_var, ":"), name)))
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
