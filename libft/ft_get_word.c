#include "libft.h"

char    *ft_get_word(char *s, int nb)
{
    int		size;
	char	*ret;
	int		word;
	int		j;
	int		i;

	word = 0;
	i = -1;
	ret = NULL;
	if (!s)
		return (NULL);
	size = (int)ft_strlen(s);
	while (++i < size)
	{
		while (ft_isspace(s[i]) && s[i])
			i++;
		j = i;
		while (s[i] && !ft_isspace(s[i]))
			i++;
		if (j < i)
		{
			word++;
			if (word == nb)
			{
				ret = ft_strnew(i - j + 1);
				ft_strncpy(ret, (char *)s + j, (i - j));
				return (ret);
			}
		}
	}
	return (NULL);
}