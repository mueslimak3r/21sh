/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/25 03:25:37 by calamber          #+#    #+#             */
/*   Updated: 2019/12/05 20:18:43 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

int		dup_close(int fd1, int fd2)
{
	int		a;

	ft_printf_fd(STDERR_FILENO, "dup close %d\n", fd1);
	if ((a = dup2(fd1, fd2)) == -1)
	{
		ft_printf_fd(STDERR_FILENO, "-wtsh: Bad file descriptor: %d\n", fd1);
		return (0);
	}
	if (fd1 > 2)
		close(fd1);
	return (1);
}

void	reg_close(int fd, t_redir *list)
{
	int		n;

	if (!list)
		return ;
	n = fd;
	ft_printf_fd(STDERR_FILENO, "reg close %d\n", fd);
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
	else if (tmp)
	{
		if (!tmp || !dup_close(tmp->dst, tmp->src))
			return (0);
	}
	return (r);
}

int		execute_command(int *in, int *out, char **args, t_redir *list)
{
	pid_t	pid;

	ft_printf_fd(STDERR_FILENO, "name: %s in0: %d in1: %d out0: %d out1: %d\n", args[0], in[0], in[1], out[0], out[1]);
	in[1] > 2 ? close(in[1]) : 0;
	if ((pid = fork()) == 0)
		subshell(in, out, args, list);
	set_sighandle();
	in[0] > 2 ? close(in[0]) : 0;
	//close(in[0]);
	//out[1] > 2 ? close(out[1]) : 0;
	g_term.pid = pid;
	return (1);
}

int		empty_buffer(int fd[2])
{
	char	buf[42];
	int		read_bytes;
	//ft_printf_fd(STDERR_FILENO, "printing buff\n");
	if (fd[0] != 0)
	{
		if (fd[1] > 2)
			close(fd[1]);
		while ((read_bytes = read(fd[0], &buf, 41)) > 0)
		{
			buf[read_bytes] = 0;
			ft_printf_fd(STDERR_FILENO, "%s", buf);
		}
		if (fd[0] > 2)
			close(fd[0]);
	}
	ft_printf_fd(STDERR_FILENO, "\n");
	return (1);
}
