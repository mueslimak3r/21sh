/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/25 03:25:37 by calamber          #+#    #+#             */
/*   Updated: 2019/11/21 03:03:41 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

int		g_in;
int		g_out;
int		g_err;

int		dup_close(int fd1, int fd2)//, t_redir *list)
{
	int		a;

	if ((a = dup2(fd1, fd2)) == -1)
	{
		ft_printf_fd(STDERR_FILENO, "-wtsh: Bad file descriptor: %d\n", fd1);
		return (0);
	}
	//if (fd1 > 2)
	close(fd1);
	return (1);
}

void	reg_close(int fd, t_redir *list)
{
	int		n;

	if (!list)
		return ;
	n = fd;
	ft_printf("closing %d\n", fd);
	close(n);
	ft_printf("closed %d\n", fd);
}

int		handle_redirs(t_redir *list)
{
	t_redir *tmp;
	int		r;

	r = 1;
	tmp = list;
	if (!tmp)
		return (1);
	if (tmp && tmp->next)
		r = handle_redirs(tmp->next);
	if (tmp->dst == -1)
		reg_close(tmp->src, list);
	else if (tmp)
	{
		if (!tmp || !dup_close(tmp->dst, tmp->src))
			return (0);
	}
	return (r);
}

int		execute_command(int in, int out, char **args, t_redir *list)
{
	char	*name;
	pid_t	pid;
	name = NULL;
	args[0] = find_alias(args[0]);
	ft_printf_fd(STDERR_FILENO, "in %d out %d\n", in, out);
	if (check_path(&name, args, g_term.env.envp))
	{
		ft_printf_fd(STDERR_FILENO, "name: %s\n", name);
		if ((pid = fork()) == 0)
		{
			dup2(in, 0);
			dup2(out, 1);
			if (!handle_redirs(list))
				return (0);
			if (execve(name, args, g_term.env.envp) == -1)
			{
				exit(EXIT_SUCCESS);
			}
			exit(0);
		}
		waitpid(pid, 0, 0);
		if (in > 2)
			close(in);
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
	if (fd[0] > 2)
		close(fd[0]);
	return (1);
}
