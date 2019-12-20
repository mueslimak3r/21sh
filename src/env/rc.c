/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rc.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/16 13:23:57 by alkozma           #+#    #+#             */
/*   Updated: 2019/12/19 15:51:33 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

int		read_rcfile(void)
{
	int		fd;
	char	*line;
	t_stats	stats;
	t_node	*tree;

	fd = open(".ftshrc", O_RDONLY);
	line = NULL;
	while (get_next_line(fd, &line) > 0)
	{
		if (line[0] != '#')
		{
			stats.f_d[0] = 0;
			stats.f_d[1] = 1;
			tree = lexer(line);
			recurse(tree, &stats);
			if (g_term.children)
				child_wait();
			empty_buffer(stats.f_d);
			clean_tree(tree);
		}
		ft_memdel((void**)&line);
	}
	free(line);
	close(fd);
	return (1);
}
