/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_jump.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/10 11:14:31 by alkozma           #+#    #+#             */
/*   Updated: 2019/12/13 03:53:08 by calamber         ###   ########.fr       */
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

void	jump_by_row(t_tbuff *buff, unsigned long code)
{
	int pos;

	pos = calc_pos();
	if (code && buff)
	{
		;
	}
	//ft_printf_fd(STDERR_FILENO, "\nbefore pos %d, size %d\n", pos, g_term.conf.termsize[0]);
	if (code == KEY_ALT_UP)
	{
		if (pos - g_term.conf.termsize[0] >= 0)
		{
			move_cursor(-(g_term.conf.termsize[0]), 1, buff, -1);
		}
		else
			move_cursor(pos > 0 ? -(pos) : 0, 1, buff, -1);
	}
	else if (code == KEY_ALT_DOWN)
	{
		if (pos + g_term.conf.termsize[0] <= buff->len)
		{
			move_cursor(g_term.conf.termsize[0], 1, buff, -1);
		}
		else
			move_cursor(buff->len, 1, buff, -1);
	}
	//pos = calc_pos();
	//ft_printf_fd(STDERR_FILENO, "after pos %d, size %d\n", pos, g_term.conf.termsize[0]);
}
