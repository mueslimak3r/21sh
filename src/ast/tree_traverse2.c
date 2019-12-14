/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_traverse2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/13 19:29:32 by calamber          #+#    #+#             */
/*   Updated: 2019/12/13 19:29:35 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

int				find_pipe_r(t_node *head)
{
	t_node	*tmp;
	int		ret;

	ret = 0;
	if (!head)
		return (0);
	tmp = head;
	while (tmp)
	{
		if (tmp->children && (ret = find_pipe_r(tmp->children)) != 0)
			return (ret);
		if (tmp->lexeme && (tmp->lexeme->set == SEMI ||
							tmp->lexeme->set == AND))
			return (-1);
		if (tmp->lexeme && (tmp->lexeme->set == LESS ||
					tmp->lexeme->set == GREAT ||
					tmp->lexeme->set == RDLESS ||
					tmp->lexeme->set == RDGREAT ||
					tmp->lexeme->set == PIPE))
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

int				find_pipe(t_node *head)
{
	t_node	*tmp;
	int		ret;

	if (!head)
		return (0);
	if ((ret = find_pipe_r(head)) != 0)
		return (ret);
	if (head->parent)
	{
		tmp = head->parent->next;
		if (tmp && (ret = find_pipe(tmp)) != 0)
			return (ret);
	}
	return (0);
}

void			exec_handle_redir(t_node *node, int *in, int *out)
{
	if (node->children->set == FD_W)
	{
		readfd(in[0], open(node->children->lexeme->data,
			O_WRONLY | O_CREAT | O_TRUNC, 0644), 1);
		in[0] > 2 ? close(in[0]) : 0;
		out[0] > 2 ? close(out[0]) : 0;
		out[1] > 2 ? close(out[1]) : 0;
		out[0] = 0;
		out[1] = 1;
	}
	else if (node->children->set == FD_A)
	{
		readfd(in[0], open(node->children->lexeme->data,
			O_WRONLY | O_CREAT | O_APPEND, 0644), 1);
		in[0] > 2 ? close(in[0]) : 0;
		out[0] > 2 ? close(out[0]) : 0;
		out[1] > 2 ? close(out[1]) : 0;
		out[0] = 0;
		out[1] = 1;
	}
	else
	{
		exec_heredoc(node, in, out);
		out[1] > 2 ? close(out[1]) : 0;
	}
}
