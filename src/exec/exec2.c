/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/25 03:25:37 by calamber          #+#    #+#             */
/*   Updated: 2019/11/21 00:41:19 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

int		g_in;
int		g_out;
int		g_err;

int		dup_close(int fd1, int fd2)//, t_redir *list)
{
	int		n;
	int		a;

	//if (!list)
	//	return (0);
	n = fd1;
	if ((a = dup2(n, fd2)) == -1)
	{
		ft_printf_fd(STDERR_FILENO, "-wtsh: Bad file descriptor: %d\n", n);
		return (0);
	}
	//ft_printf("%d\n", a);
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
	else
	{
		if (!list || !dup_close(tmp->dst, tmp->src))
			return (0);
	}
	return (r);
}

void	mod_redirs(t_redir *list, int in, int out, int err)
{
	t_redir *tmp;

	tmp = list;
	while (tmp)
	{
		if (tmp->src == 0)
			tmp->src = in;
		if (tmp->src == 1)
			tmp->src = out;
		if (tmp->src == 2)
			tmp->src = err;
		if (tmp->dst == 0)
			tmp->dst = in;
		if (tmp->dst == 1)
			tmp->dst = out;
		if (tmp->dst == 2)
			tmp->dst = err;
		tmp = tmp->next;
	}
}

int		execute_command(int in, int out, int err, char **args, t_redir *list)
{
	char	*name;
	pid_t	pid;
	//int		restore[3];

	name = NULL;
	/*
	restore[0] = dup(in);
	restore[1] = dup(out);
	restore[2] = dup(err);
	g_in = restore[0];
	g_out = restore[1];
	g_err = restore[2];
	*/
	if (err || in || out)
	{
		;
	}
	args[0] = find_alias(args[0]);
	ft_printf_fd(STDERR_FILENO, "in %d out %d err %d\n", in, out, err);
	if (check_path(&name, args, g_term.env.envp))
	{
		/*
		add_redir(0, restore[0], &list);
		add_redir(1, restore[1], &list);
		add_redir(2, restore[2], &list);
		*/
		if ((pid = fork()) == 0)
		{
			//mod_redirs(list, restore[0], restore[1], restore[2]);
			
			dup2(in, 0);
			dup2(out, 1);
			//dup2(2, err);
			//if (in > 0)
			//	dup_close(in, STDIN_FILENO);
			if (!handle_redirs(list))
				return (0);
			//if (in < 0)
			//	close(0);
			//else if (out < 0)
			//	close(1);
			//else if (err < 0)
			//	close(2);

			//if (out != 1 && out > 0)
			//	dup_close(out, STDOUT_FILENO);
			//if (err != 2 && out > 0)
			//	dup_close(err, STDERR_FILENO);
			if (execve(name, args, g_term.env.envp) == -1)
			{
				if (in > 0)
					close(in);
				exit(EXIT_SUCCESS);
			}
			if (in > 0)
				close(in);
			exit(0);
		}

		waitpid(pid, 0, 0);
		/*
		dup2(restore[0], in);
		dup2(restore[1], out);
		dup2(restore[2], err);
		close(restore[0]);
		close(restore[1]);
		close(restore[2]);
		*/
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
