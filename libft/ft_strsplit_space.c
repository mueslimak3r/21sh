#include "libft.h"

char	**ft_strsplit_space(char const *s)
{
	int		h;
	int		i;
	int		j;
	char	**p;

	h = 0;
	i = -1;
	if (!s)
		return (NULL);
	if (!(p = (char **)ft_memalloc(sizeof(char *) * (ft_cntwords_space(s) + 1))))
		return (NULL);
	while (!(p[h] = NULL) && (++i) < (int)ft_strlen(s))
	{
		while (ft_isspace(s[i]) && s[i])
			i++;
		j = i;
		while (s[i] && !ft_isspace(s[i]))
			i++;
		if (j < i)
		{
			p[h] = ft_strnew(i - j + 1);
			ft_strncpy(p[h++], (char *)s + j, (i - j));
		}
	}
	return (p);
}