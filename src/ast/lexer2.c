/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/19 16:16:49 by calamber          #+#    #+#             */
/*   Updated: 2019/12/19 16:23:45 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

char		*find_quote_match(char *data, char ref)
{
	char	*res;
	char	*data_cpy;

	data_cpy = data;
	while (data_cpy)
	{
		res = ft_strchr(data_cpy + 1, ref);
		if (res && !(res - data_cpy > 1 && *(res - 1) == '\\'))
			break ;
		data_cpy = res;
		res = NULL;
	}
	return (res);
}

int			cpy_quote_substr(char *data, char *ret, int cur, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		if (data[i] == '\\')
		{
			if (i < size - 1 && data[i + 1])
				ret[cur++] = data[i++ + 1];
			i++;
		}
		else
			ret[cur++] = data[i++];
	}
	return (cur);
}

int			quote_parser(char *data, char **ret, int *i, int *cur)
{
	char	*res2;

	res2 = find_quote_match(data + *i, data[*i]);
	if (res2)
	{
		(*i)++;
		*cur = cpy_quote_substr(data + *i, *ret, *cur,
							(long)res2 - (long)(data + *i));
		*i += (long)res2 - (long)(data + *i) + 1;
	}
	else
	{
		ft_memdel((void**)ret);
		ft_printf_fd(STDERR_FILENO, "-wtsh: quote error\n");
		return (0);
	}
	return (1);
}
