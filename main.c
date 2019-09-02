#include "21sh.h"

int	main(void)
{
	char	buf[5000];

	ft_bzero(buf, 5000);
	while (read(0, &buf, 5000) >= 0)
	{
		parse_input(buf);
	}
}
