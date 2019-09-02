#include "ftshell.h"

static void		sig_resume(int nb)
{
	if (nb)
	{
		init_term();
		set_sighandle();
		CLEAR_SCREEN;
	}
}

static void		sig_suspend(int nb)
{
	if (nb)
	{
		reset_term();
		CLEAR_SCREEN;
		signal(SIGTSTP, SIG_DFL);
		ioctl(STDERR_FILENO, TIOCSTI, "\x1A");
	}
}

static void		sig_resize(int nb)
{
	if (nb)
	{
		CLEAR_SCREEN;
		GET_SCREENSIZE;
	}
}

static void		sig_stop(int nb)
{
	if (nb)
	{
		reset_term();
		exit(0);
	}
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
	signal(SIGINT, sig_stop);
	signal(SIGABRT, sig_stop);
}
