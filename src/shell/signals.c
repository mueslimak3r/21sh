/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/25 04:10:16 by calamber          #+#    #+#             */
/*   Updated: 2019/12/18 00:24:49 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

void		sig_resume(int nb)
{
	if (nb)
	{
		//init_term();
		//set_sighandle();
		//g_term.sigs.restart = true;
	}
}

void		sig_suspend(int nb)
{
	if (nb)
	{
		//reset_term();
		//signal(SIGTSTP, SIG_DFL);
		//ioctl(STDERR_FILENO, TIOCSTI, "\x1A");
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
