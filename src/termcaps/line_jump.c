#include "ftshell.h"

int	jump_by_word_amt(char *str, int pos, int dir)
{
	int	i;

	i = 0;
	if (pos == 0)
		return (0);
	if (dir > 0)
		return ((int)(ft_strchr(str, ' ') - str));
	else
	{
		i = pos;
		while (i && str[i] != ' ')
			i--;
	}
	return (i);
}
