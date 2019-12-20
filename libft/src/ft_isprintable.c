/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isprintable.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/19 15:37:10 by calamber          #+#    #+#             */
/*   Updated: 2019/12/19 15:37:11 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int			ft_isprintable(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (!ft_isalnum(s[i]) && !(s[i] == ' ' || s[i] == '\n' || s[i] == '\t')
			&& !(s[i] >= '!' && s[i] <= '/') && !(s[i] >= ':' && s[i] <= '@')
			&& !(s[i] >= '[' && s[i] <= '`') && !(s[i] >= '{' && s[i] <= '~'))
			return (0);
		i++;
	}
	return (1);
}
