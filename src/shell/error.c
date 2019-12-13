/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkozma <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/10 18:27:23 by alkozma           #+#    #+#             */
/*   Updated: 2019/12/13 13:02:47 by alkozma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

int		parse_error(t_node *head, t_lexeme *error)
{
	if (error)
		ft_printf_fd(STDERR_FILENO,
				"-wtsh: syntax error near unexpected token `%s'\n",
				error->data);
	if (head)
		clean_tree(head);
	return (-1);
}
