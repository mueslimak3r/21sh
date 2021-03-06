/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 18:57:29 by calamber          #+#    #+#             */
/*   Updated: 2019/12/13 11:47:32 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

void			set_sighandle_child(void)
{
	signal(SIGFPE, SIG_DFL);
	signal(SIGILL, SIG_DFL);
	signal(SIGINT, SIG_DFL);
	signal(SIGTERM, SIG_DFL);
	signal(SIGTSTP, SIG_DFL);
	signal(SIGINT, SIG_DFL);
	signal(SIGABRT, SIG_DFL);
}

void			set_sighandle(void)
{
	signal(SIGFPE, sig_stop);
	signal(SIGILL, sig_stop);
	signal(SIGINT, sig_stop);
	signal(SIGTERM, sig_stop);
	signal(SIGWINCH, sig_resize);
	signal(SIGTSTP, sig_suspend);
	signal(SIGINT, sig_int);
	signal(SIGABRT, sig_stop);
}
