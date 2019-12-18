/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/25 04:10:16 by calamber          #+#    #+#             */
/*   Updated: 2019/12/18 01:24:39 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

void		sig_suspend(int nb)
{
	if (nb)
	{
		;
		/*
		if (g_term.conf.is_child)
		{
			reset_term();
			signal(SIGTSTP, SIG_DFL);
			kill(g_term.pid, SIGTSTP);
			raise(SIGTSTP);
			init_term();
			set_sighandle();
			if (g_term.pid != -1)
				kill(g_term.pid, SIGCONT);
			//ioctl(STDERR_FILENO, TIOCSTI, "\x1A");
		}
		*/
	}
}

void		sig_resize(int nb)
{
	if (nb)
	{
		ioctl(STDERR_FILENO, TIOCGWINSZ, &g_window_size);
	}
}

void		sig_stop(int nb)
{
	if (nb)
	{
		reset_term();
		g_term.sigs.restart = true;
		g_term.sigs.sigstop = true;
	}
}

void		sig_int(int nb)
{
	if (nb)
	{
		g_term.sigs.sigint = true;
		g_term.sigs.restart = true;
	}
}
