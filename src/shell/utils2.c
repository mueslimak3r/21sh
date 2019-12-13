/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 19:20:52 by calamber          #+#    #+#             */
/*   Updated: 2019/12/13 03:26:36 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

int				has_hd(char *thing, char *hd)
{
	char	**split;
	int		i;
	int		ret;

	if (!thing)
		return (0);
	split = ft_strsplit(thing, '\n');
	i = 0;
	ret = 0;
	while (split[i])
	{
		if (ft_strcmp(split[i], hd) == 0)
			ret = 1;
		free(split[i]);
		split[i] = NULL;
		i++;
	}
	free(split);
	split = NULL;
	return (ret);
}

int				termcap_reset_cursor(int pos, int len)
{
	int		a;

	a = len - 1;
	while (a-- > pos)
		ft_printf_fd(STDERR_FILENO, "\b");
	return (1);
}

void			shell_reset_stuff(t_stats *stats)
{
	g_term.sigs.sigint = false;
	stats->f_d[0] = 0;
	stats->f_d[1] = 1;
	g_term.pid = -1;
	g_term.hd_buff = NULL;
	redo_prompt(0, 0);
	zero_cursor();
}

int				ft_charput(int c)
{
	return (write(1, &c, 1));
}

unsigned long	djb2(char *str)
{
	unsigned long	hash;
	int				c;

	hash = 5381;
	while ((c = *str++))
		hash = ((hash << 5) + hash) + c;
	return (hash);
}
