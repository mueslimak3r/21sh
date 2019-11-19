/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/25 03:25:37 by calamber          #+#    #+#             */
/*   Updated: 2019/11/18 20:41:19 by alkozma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

void	dup_close(int fd1, int fd2, t_redir *list)
{
	t_redir	*tmp;
	int		n;

	n = fd1;
	tmp = list;
	while (tmp)
	{
		if (tmp->src == n && n > 0 && fd2 != tmp->dst)
			n = tmp->dst;
		tmp = tmp->next;
	}
	dup2(n, fd2);
	close(n);
	ft_printf_fd(STDERR_FILENO, "duping %d %d\n", n, fd2);
}

void	reg_close(int fd, t_redir *list)
{
	t_redir *tmp;
	int		n;

	tmp = list;
	n = fd;
	while (tmp)
	{
		if (tmp->src == n && tmp->dst > 0)
			n = tmp->dst;
		tmp = tmp->next;
	}
	ft_printf_fd(STDERR_FILENO, "closing %d\n", n);
	close(n);
}

void	handle_redirs(t_redir *list)
{
	t_redir *tmp;

	tmp = list;
	if (!tmp)
		return ;
	if (tmp && tmp->next)
		handle_redirs(tmp->next);
	if (tmp->dst == -1)
		reg_close(tmp->src, list);
	else
		dup_close(tmp->src, tmp->dst, list);
	tmp = tmp->next;
}

int		execute_command(int in, int out, int err, char **args, t_redir *list)
{
	char	*name;
	int		io[2];
	pid_t	pid;

	name = NULL;
	args[0] = find_alias(args[0]);
	io[0] = in;
	io[1] = out;
	if (check_path(&name, args, g_term.env.envp))
	{
		if ((pid = fork()) == 0)
		{
			if (in > 0)
				add_redir(in, 0, &list);
			if (out != 1)
				add_redir(out, 1, &list);
			if (err != 2)
				add_redir(err, 2, &list);
			handle_redirs(list);
			/*
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
			*/
			ft_printf("[%d][%d][%d]\n", in, out, err);
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
