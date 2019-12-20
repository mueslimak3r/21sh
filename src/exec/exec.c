/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/09 10:49:27 by alkozma           #+#    #+#             */
/*   Updated: 2019/12/11 14:36:56 by alkozma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

int		exec_size(char *line)
{
	return ((ft_strchr(line, ':') < line) ? 0 : (ft_strchr(line, ':') - line));
}

int		find_exec(char *arg, char *envp, char **result)
{
	char			*line;
	char			*path;
	int				size;

	if (resolve_path(arg))
		return ((*result = ft_strdup(arg)) ? 1 : 0);
	if (!(line = envp))
		return (0);
	while (*line)
	{
		if (!(size = exec_size(line)))
			break ;
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
	int		number = 0;

	if (fd2 == -1 || fd1 == -1)
		ft_printf_fd(STDERR_FILENO, "%s\n", strerror(errno));
	ft_memset(buf, 0, 42);
	while ((bytes = read(fd1, buf, 41)) > 0)
	{
		buf[bytes] = 0;
		number += bytes;
		write(fd2, buf, bytes);
		ft_memset(buf, 0, 42);
	}
	c2 && fd2 > 2 ? close(fd2) : 0;
	!c2 && fd1 > 2 ? close(fd1) : 0;
	return (1);
}
