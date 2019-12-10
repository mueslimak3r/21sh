/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_jump.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkozma <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/10 11:14:31 by alkozma           #+#    #+#             */
/*   Updated: 2019/12/10 11:44:37 by alkozma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

int	jump_by_word_amt(char *str, int pos, int dir)
{
	int	i;
	int	len;

	i = 0;
	if (pos == 0 && dir < 0)
		return (0);
	if (dir > 0)
	{
		i = pos + 1;
		len = (int)ft_strlen(str);
		while (i < len && str[i] != ' ')
			i++;
	}
	else
	{
		i = pos - 1;
		while (i && str[i] != ' ')
			i--;
	}
	return (i);
}
