#include "ft_shell.h"

static bool	validate_term(void)
{
	char	*term_id;
	char	buf[2048];

	term_id = getenv("TERM");
	if (!term_id)
		ft_putstr_fd("error: $TERM variable invalid\n",
		STDERR_FILENO);
	else if (!tgetent(buf, term_id))
		ft_putstr_fd("error: tgetent failed\n",
		STDERR_FILENO);
	else
		return (true);
	return (false);
}

void		init_term(void)
{
	char	buf[30];
	char	*temp;

	temp = buf;
	tcgetattr(STDERR_FILENO, &terms.old_term);
	tcgetattr(STDERR_FILENO, &terms.new_term);
	terms.new_term.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDERR_FILENO, TCSANOW, &terms.new_term);
	if (!(tgetstr("vi", &temp)))
	{
		ft_putstr_fd("error: host terminal attr invalid\n", STDERR_FILENO);
		exit (0);
	}
	ft_putstr_fd(tgetstr("vi", &temp), STDERR_FILENO);
	ft_putstr_fd(tgetstr("ti", &temp), STDERR_FILENO);
	GET_SCREENSIZE;
	terms.rows = 1;
}

void		reset_term(void)
{
	char	*temp;
	char	buf[30];

	temp = buf;
	tcsetattr(STDERR_FILENO, TCSANOW, &terms.old_term);
	ft_putstr_fd(tgetstr("ve", &temp), STDERR_FILENO);
	ft_putstr_fd(tgetstr("te", &temp), STDERR_FILENO);
}

int			main(int ac, char **av)
{

	if (ac < 2 || !(validate_term()))
		return (0);
	init_term();
	set_sighandle();
	reset_term();
	return (0);
}