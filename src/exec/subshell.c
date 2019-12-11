/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/09 11:38:47 by alkozma           #+#    #+#             */
/*   Updated: 2019/12/11 14:36:02 by alkozma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

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

	name = NULL;
	args[0] = find_alias(args[0]);
	out[0] > 2 ? close(out[0]) : 0;
	set_sighandle_child();
	if (!check_path(&name, args, g_term.env.envp))
		return (ft_printf_fd(2, "-wtsh: %s: command not found\n", args[0]));
	reset_term();
	if ((pid = fork()) == 0)
	{
		fd_helper(in, out);
		if (!handle_redirs(list))
			return (0);
		execve(name, args, g_term.env.envp);
		exit(0);
	}
	out[1] > 2 ? close(out[1]) : 0;
	waitpid(pid, 0, 0);
	init_term();
	name ? free(name) : 0;
	return (0);
}
