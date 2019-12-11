/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_redir2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 22:48:44 by calamber          #+#    #+#             */
/*   Updated: 2019/12/11 11:03:06 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

void	add_redir(int src, int dst, t_redir **list)
{
	t_redir *tmp;
	t_redir	*tail;

	tmp = ft_memalloc(sizeof(*tmp));
	tmp->src = src;
	tmp->dst = dst;
	tail = *list;
	while (tail && tail->next)
		tail = tail->next;
	if (tail)
		tail->next = tmp;
	else
		*list = tmp;
	tmp->next = NULL;
}
