/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/25 04:10:16 by calamber          #+#    #+#             */
/*   Updated: 2019/11/21 19:18:04 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

void		sig_resume(int nb)
{
	if (nb)
	{
		init_term();
		set_sighandle();
		if (g_term.curr_buff)
			reprint_buffer(g_term.curr_buff);
	}
}

void		sig_suspend(int nb)
{
	if (nb)
	{
		reset_term();
		signal(SIGTSTP, SIG_DFL);
		ioctl(STDERR_FILENO, TIOCSTI, "\x1A");
	}
}

void		sig_resize(int nb)
{
	if (nb)
	{
		GET_SCREENSIZE;
	}
}

void		sig_stop(int nb)
{
	if (nb)
	{
		reset_term();
		exit(0);
	}
}

void		sig_int(int nb)
{
	char c;

	c = 4;
	if (nb)
	{
		g_term.sigs.sigint = true;
	}
}
