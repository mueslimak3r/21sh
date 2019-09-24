/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_path.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/24 09:56:23 by calamber          #+#    #+#             */
/*   Updated: 2019/09/24 06:24:05 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

int			run_command(char *name, char **args, char **envp)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		if (execve(name, args, envp) == -1)
			ft_putstr_fd("don't talk to me like that!\n", 2);
		exit(EXIT_FAILURE);
	}
	else if (pid < 0)
	{
		ft_putstr_fd("fork malfunction. Please use spoon\n", 2);
		exit(EXIT_FAILURE);
	}
	wait(&status);
	return (status);
}

int			run_dispatch(char **args, t_env *env)
{
	int		ret;
	char	*name;

	name = 0;
	if (!(*args))
		return (0);
	//if ((ret = run_builtins(args, env)) != 2)
	//	return (ret);
	ret = 0;
	if (check_path(&name, args, env->envp))
	{
		ret = run_command(name, args, env->envp);
		free(name);
	}
	else
		ret = run_command(args[0], args, env->envp);
	return (ret);
}

int					check_dir(char *name, char *path)
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

int					find_exec(char *arg, char *envp, char **result)
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

int					get_env(char **envp, char *name, char **env_var)
{
	size_t			size;

	if (!name || !envp)
		return (0);
	if ((find_env(envp, name)) == -1)
		return (0);
	size = ft_strlen(name);
	while (*envp && ft_strncmp(*envp, name, size) != 0)
		envp++;
	if (*envp && ft_strncmp(*envp, name, size) != 0)
		return (0);
	*env_var = ft_strdup(*envp + size + 1);
	return (1);
}

int					check_path(char **name, char **args, char **envp)
{
	char			*env_var;

	env_var = 0;
	if (!get_env(envp, "PATH", &env_var))
		return (0);
	if (!(find_exec(*args, ft_strjoin(env_var, ":"), name)))
		return (ft_returnfree(&env_var, 0));
	return (ft_returnfree(&env_var, 1));
}

int					make_env(t_env *env)
{
	int				i;
	extern char		**environ;
	char			**ret;

	i = 0;
	while (environ[i])
		i++;
	if (!(ret = ft_memalloc(sizeof(char*) * (i + 1))))
		return (0);
	i = 0;
	while (environ[i])
	{
		ret[i] = ft_strdup(environ[i]);
		i++;
		env->size++;
	}
	env->envp = ret;
	return (1);
}
