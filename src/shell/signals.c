/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/25 04:10:16 by calamber          #+#    #+#             */
/*   Updated: 2019/12/18 08:27:52 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

void		sig_suspend(int nb)
{
	if (nb)
	{
		;
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
