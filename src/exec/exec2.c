/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/25 03:25:37 by calamber          #+#    #+#             */
/*   Updated: 2019/11/21 22:36:42 by calamber         ###   ########.fr       */
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

int		execute_command(int in, int out, char **args, t_redir *list)
{
	char	*name;
	pid_t	pid;

	name = NULL;
	args[0] = find_alias(args[0]);
	if (!check_path(&name, args, g_term.env.envp))
		return (ft_printf_fd(2, "-wtsh: %s: command not found\n", args[0]));
	reset_term();
	if ((pid = fork()) == 0)
	{
		dup2(in, 0);
		dup2(out, 1);
		if (!handle_redirs(list))
			return (0);
		set_sighandle_child();
		execve(name, args, g_term.env.envp);
		exit(0);
	}
	waitpid(pid, 0, 0);
	init_term();
	set_sighandle();
	in > 2 ? close(in) : 0;
	name ? free(name) : 0;
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
