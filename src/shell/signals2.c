/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 18:57:29 by calamber          #+#    #+#             */
/*   Updated: 2019/11/21 18:57:31 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

void			set_sighandle_child(void)
{
	signal(SIGFPE, SIG_DFL);
	signal(SIGILL, SIG_DFL);
	signal(SIGINT, SIG_DFL);
	signal(SIGSEGV, SIG_DFL);
	signal(SIGTERM, SIG_DFL);
	signal(SIGCONT, SIG_DFL);
	signal(SIGTSTP, SIG_DFL);
	signal(SIGINT, SIG_DFL);
	signal(SIGABRT, SIG_DFL);
}

void			set_sighandle(void)
{
	signal(SIGFPE, sig_stop);
	signal(SIGILL, sig_stop);
	signal(SIGINT, sig_stop);
	signal(SIGSEGV, sig_stop);
	signal(SIGTERM, sig_stop);
	signal(SIGWINCH, sig_resize);
	signal(SIGCONT, sig_resume);
	signal(SIGTSTP, sig_suspend);
	signal(SIGINT, sig_int);
	signal(SIGABRT, sig_stop);
}
