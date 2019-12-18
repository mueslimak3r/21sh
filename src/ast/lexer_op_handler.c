/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_op_handler.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/18 19:12:05 by calamber          #+#    #+#             */
/*   Updated: 2019/12/18 09:11:49 by alkozma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

int			is_operator(char *op, int pos)
{
	int		i;
	int		ret;

	i = 3;
	ret = 0;
	if (is_redirect(op + pos))
		return (12);
	if (is_nb_before_redir(op + pos))
		return (IO_NAME);
	while (g_term.symbls[i] && i < 12)
	{
		if (ft_strncmp(op + pos, g_term.symbls[i],
			ft_strlen(g_term.symbls[i])) == 0)
			return (i);
		i++;
	}
	if (op && ret == 0)
		return (1);
	return (ret);
}
