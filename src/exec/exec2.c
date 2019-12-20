/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/25 03:25:37 by calamber          #+#    #+#             */
/*   Updated: 2019/12/19 15:38:34 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

int		dup_close(int fd1, int fd2)
{
	int		a;

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

	if ((pid = fork()) == 0)
	{
		subshell(in, out, args, list);
		exit(0);
	}
	child_push(&g_term.children, pid);
	out[1] > 2 ? close(out[1]) : 0;
	in[0] > 2 ? close(in[0]) : 0;
	set_sighandle();
	free_redir(&list);
	return (1);
}

int		empty_buffer(int fd[2])
{
	char	buf[42];
	int		read_bytes;

	if (fd[0] != 0)
	{
		while ((read_bytes = read(fd[0], buf, 41)) > 0)
		{
			buf[read_bytes] = 0;
			ft_printf_fd(STDERR_FILENO, "%s", buf);
		}
		if (fd[0] > 2)
			close(fd[0]);
	}
	return (1);
}
