/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkozma <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/09 10:49:27 by alkozma           #+#    #+#             */
/*   Updated: 2019/10/11 16:18:22 by alkozma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

int		execute_command(t_node *a, int in, int out)
{
	char	*name;
	char	**args;
	pid_t	pid;
	
	args = concat_node(a);
	if (check_path(&name, args, g_term.env.envp))
	{
		if ((pid = fork()) == 0)
		{
			if (in)
			{
				dup2(in, STDIN_FILENO);
				close(in);
			}
			if (out != 1)
			{
				dup2(out, STDOUT_FILENO);
				close(out);
			}
			if (execve(name, args, g_term.env.envp) == -1)
				exit(EXIT_SUCCESS);
			exit(0);
		}
		waitpid(pid, 0, 0);
	}
	return (1);
}

int		empty_buffer(int fd[2])
{
	char	buf[42];
	int		read_bytes;

	if (fd[0] == 0)
		return (0);
	while ((read_bytes = read(fd[0], &buf, 41)) >= 0)
	{
		buf[read_bytes] = 0;
		ft_printf_fd(STDERR_FILENO, "%s", buf);
		if (read_bytes == 0)
		{
			write(STDERR_FILENO, "\n", 1);
			break;
		}
	}
	return (1);
}

int		exec_pipe(t_node *a, t_node *b)
{
	char	*name_a;
	char	*name_b;
	char	**args_a;
	char	**args_b;
	pid_t	pid;
	int		fd[2];

	args_a = concat_node(a);
	args_b = concat_node(b);
	if (check_path(&name_a, args_a, g_term.env.envp)
			&& check_path(&name_b, args_b, g_term.env.envp))
	{
		if (pipe(fd))
			return (ft_printf_fd(STDERR_FILENO, "pipe error\n"));
		if ((pid = fork()) == 0)
		{
			dup2(fd[1], STDOUT_FILENO);
			close(fd[0]);
			if (execve(name_a, args_a, g_term.env.envp) == -1)
			{
				ft_printf_fd(STDERR_FILENO, "flug\n");
				exit(EXIT_SUCCESS);
			}
			close(fd[1]);
			exit(0);
		}
		if ((pid = fork()) == 0)
		{
			dup2(fd[0], STDIN_FILENO);
			close(fd[1]);
			if (b->next && b->next->lexeme && b->next->lexeme->set == PIPE)
				exec_pipe(b, b->next->next);
			else
			{
				if (execve(name_b, args_b, g_term.env.envp) == -1)
				{
					ft_printf_fd(STDERR_FILENO, "flug\n");
					exit(EXIT_SUCCESS);
				}
			}
			close(fd[0]);
			exit(0);
		}
		close(fd[0]);
		close(fd[1]);
		waitpid(pid, 0, 0);
	}
	return (1);
}
