/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_redir2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 22:48:44 by calamber          #+#    #+#             */
/*   Updated: 2019/11/21 22:48:47 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

void	add_redir(int src, int dst, t_redir **list)
{
	t_redir *tmp;

	tmp = ft_memalloc(sizeof(*tmp));
	tmp->src = src;
	tmp->dst = dst;
	tmp->next = *list;
	*list = tmp;
}
