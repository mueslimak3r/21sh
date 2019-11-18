/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/25 03:25:37 by calamber          #+#    #+#             */
/*   Updated: 2019/11/17 12:22:37 by alkozma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

void	dup_close(int fd1, int fd2)
{
	dup2(fd1, fd2);
	close(fd1);
}

int		execute_command(int in, int out, int err, char **args)
{
	char	*name;
	pid_t	pid;

	name = NULL;
	args[0] = find_alias(args[0]);
	if (check_path(&name, args, g_term.env.envp))
	{
		if ((pid = fork()) == 0)
		{
			ft_printf("[%d][%d][%d]\n", in, out, err);
			if (in < 0)
				close(0);
			else if (out < 0)
				close(1);
			else if (err < 0)
				close(2);
			if (in > 0)
				dup_close(in, STDIN_FILENO);
			if (out != 1 && out > 0)
				dup_close(out, STDOUT_FILENO);
			if (err != 2 && out > 0)
				dup_close(err, STDERR_FILENO);
			if (execve(name, args, g_term.env.envp) == -1)
				exit(EXIT_SUCCESS);
			exit(0);
		}
		waitpid(pid, 0, 0);
	}
	if (name)
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
