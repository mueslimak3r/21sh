/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkozma <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/09 10:49:27 by alkozma           #+#    #+#             */
/*   Updated: 2019/10/11 17:28:01 by alkozma          ###   ########.fr       */
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
			break;
	}
	return (1);
}
