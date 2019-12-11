/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_jump.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/10 11:14:31 by alkozma           #+#    #+#             */
/*   Updated: 2019/12/11 14:26:03 by calamber         ###   ########.fr       */
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
		while (i < len && str[i] == ' ')
			i++;
		while (i < len && str[i] != ' ')
			i++;
	}
	else
	{
		i = pos - 1;
		while (i && str[i] == ' ')
			i--;
		while (i && str[i] != ' ')
			i--;
	}
	return (i);
}

void	jump_by_row(t_tbuff *buff, int code)
{
	int pos;

	pos = calc_pos();
	if (code == ALTUP)
	{
		if (g_term.conf.cursor[1] > 0)
		{
			move_cursor(-(g_term.conf.termsize[0]), 1, buff);
		}
	}
	else if (code == ALTDOWN)
	{
		move_cursor(g_term.conf.termsize[0], 1, buff);
	}
}