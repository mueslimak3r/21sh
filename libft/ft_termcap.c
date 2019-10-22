#include "libft.h"

void		ft_termcap(char *cmd, int fd)
{
	char	*temp;
	char	buf[30];

	temp = buf;
	if (!cmd || fd < 1)
		return ;
	ft_putstr_fd(tgetstr(cmd, &temp), fd);
}