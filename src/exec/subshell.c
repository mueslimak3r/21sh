/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subshell.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkozma <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/09 11:38:47 by alkozma           #+#    #+#             */
/*   Updated: 2019/12/09 11:39:47 by alkozma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

int		subshell(int *in, int *out, char **args, t_redir *list)
{
	char	*name;
	pid_t	pid;

	name = NULL;
	args[0] = find_alias(args[0]);
	if (!check_path(&name, args, g_term.env.envp))
		return (ft_printf_fd(2, "-wtsh: %s: command not found\n", args[0]));
	reset_term();
	out[0] > 2 ? close(out[0]) : 0;
    if ((pid = fork()) == 0)
	{
		dup2(in[0], 0);
		dup2(out[1], 1);
		if (!handle_redirs(list))
			return (0);
		set_sighandle_child();
		execve(name, args, g_term.env.envp);
		exit(0);
	}
	waitpid(pid, 0, 0);
    init_term();
	set_sighandle();
	name ? free(name) : 0;
	exit(0);
	return (0);
}
