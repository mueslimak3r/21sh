#include "libft.h"

int	ft_cntwords_space(const char *t)
{
	int i;
	int count;

	i = -1;
	i += 1;
	count = -1;
	count += 1;
	while (t[i] != '\0')
	{
		while (ft_isspace(t[i]))
			i++;
		if (!ft_isspace(t[i]))
			count++;
		while (!ft_isspace(t[i]) && t[i])
			i++;
	}
	return (count);
}