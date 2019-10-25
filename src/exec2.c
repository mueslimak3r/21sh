/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/25 03:25:37 by calamber          #+#    #+#             */
/*   Updated: 2019/10/25 03:25:39 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

void	dup_close(int fd1, int fd2)
{
	dup2(fd1, fd2);
	close(fd1);
}

int		execute_command(t_node *a, int in, int out, char **args)
{
	char	*name;
	pid_t	pid;

	args[0] = find_alias(args[0]);
	if (check_path(&name, args, g_term.env.envp))
	{
		if ((pid = fork()) == 0)
		{
			if (in)
				dup_close(in, STDIN_FILENO);
			if (out != 1)
				dup_close(out, STDOUT_FILENO);
			if (execve(name, args, g_term.env.envp) == -1)
				exit(EXIT_SUCCESS);
			exit(0);
		}
		waitpid(pid, 0, 0);
	}
	free(name);
	return (1);
}

int		print_buffer(int fd[2])
{
	char	buf[42];
	int		tmpfd[2];
	int		read_bytes;

	if (fd[0] == 0)
		return (0);
	if (pipe(tmpfd) == -1)
		return (0);
	while ((read_bytes = read(fd[0], &buf, 41)) > 0)
	{
		buf[read_bytes] = 0;
		ft_printf_fd(STDERR_FILENO, "PRINTBUF: %s", buf);
		ft_printf_fd(tmpfd[1], "%s", buf);
		if (read_bytes == 0)
			break ;
	}
	close(tmpfd[1]);
	close(fd[0]);
	fd[0] = tmpfd[0];
	return (1);
}

int		empty_buffer(int fd[2])
{
	char	buf[42];
	int		read_bytes;

	if (fd[0] == 0)
		return (0);
	while ((read_bytes = read(fd[0], &buf, 41)) > 0)
	{
		buf[read_bytes] = 0;
		ft_printf_fd(STDERR_FILENO, "%s", buf);
	}
	return (1);
}
