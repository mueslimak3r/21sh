/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/09 11:38:47 by alkozma           #+#    #+#             */
/*   Updated: 2019/12/20 15:46:15 by alkozma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

void		free_redir(t_redir **list)
{
	t_redir *tmp;

	while (*list)
	{
		tmp = *list;
		*list = (*list)->next;
		free(tmp);
	}
}

static void	fd_helper(int *in, int *out)
{
	dup2(in[0], 0);
	dup2(out[1], 1);
	in[0] > 2 ? close(in[0]) : 0;
	out[1] > 2 ? close(out[1]) : 0;
}

int			subshell(int *in, int *out, char **args, t_redir *list)
{
	char	*name;
	pid_t	pid;
	int		status;

	name = NULL;
	args[0] = find_alias(args[0]);
	out[0] > 2 ? close(out[0]) : 0;
	if (!check_path(&name, args, g_term.env.envp))
		return (ft_printf_fd(2, "-wtsh: %s: command not found\n", args[0]));
	reset_term();
	if ((pid = fork()) == 0)
	{
		set_sighandle_child();
		fd_helper(in, out);
		if (!handle_redirs(list))
			return (0);
		execve(name, args, g_term.env.envp);
		exit(0);
	}
	out[1] > 2 ? close(out[1]) : 0;
	wait(&status);
	init_term();
	free(name);
	(WIFSIGNALED(status)) ? ft_printf_fd(STDERR_FILENO, "\n") : 0;
	return (WEXITSTATUS(status));
}
