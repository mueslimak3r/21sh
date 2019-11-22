/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 22:13:39 by calamber          #+#    #+#             */
/*   Updated: 2019/11/21 22:46:50 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

int		redir_left_io(int *left_io, t_node **tmp)
{
	if (!*tmp || !(*tmp)->lexeme)
		return (0);
	if ((*tmp)->lexeme->set == IO_NAME)
	{
		*left_io = ft_atoi((*tmp)->lexeme->data);
		*tmp = (*tmp)->next;
	}
	return (1);
}

int		redir_middle_helper(int *left_io, int *right_io, t_node *tmp)
{
	if (ft_strchr(tmp->lexeme->data, '&') && ft_strchr(tmp->lexeme->data, '&') <
	ft_strchr(tmp->lexeme->data, (tmp->lexeme->set == L_REDIRECT ? '<' : '>')))
	{
		if (*left_io == -1)
			*left_io = -2;
		else
		{
			ft_printf_fd(STDERR_FILENO, "redir syntax err\n");
			return (0);
		}
	}
	else if (ft_strchr(tmp->lexeme->data, '&'))
	{
		*right_io = -2;
	}
	return (1);
}

int		redir_middle(int *left_io, int *right_io, int *dir, t_node **tmp)
{
	if (!*tmp || !(*tmp)->lexeme)
		return (0);
	if ((*tmp)->lexeme->set == L_REDIRECT || (*tmp)->lexeme->set == R_REDIRECT)
	{
		*dir = (*tmp)->lexeme->set == L_REDIRECT ? -1 : 1;
		if (!redir_middle_helper(left_io, right_io, *tmp))
			return (0);
		if (*left_io == -1)
			*left_io = 1;
		*tmp = (*tmp)->next;
	}
	else
	{
		ft_printf_fd(STDERR_FILENO, "redir syntax err\n");
		return (0);
	}
	return (1);
}

void	redir_push(int left_io, int right_io, int dir, t_redir **list)
{
	if (left_io == -2)
	{
		add_redir((dir == -1 ? right_io : 1), (dir == -1 ? 1 : right_io), list);
		add_redir((dir == -1 ? right_io : 2), (dir == -1 ? 2 : right_io), list);
	}
	else if (right_io == -2)
	{
		add_redir((dir == -1 ? 1 : left_io), (dir == -1 ? left_io : 1), list);
		add_redir((dir == -1 ? 2 : left_io), (dir == -1 ? left_io : 2), list);
	}
	else
		add_redir((dir == -1 ? right_io : left_io),
		(dir == -1 ? left_io : right_io), list);
}

void	redir_pipes(t_node *node, t_redir **list)
{
	int		left_io;
	int		right_io;
	int		dir;

	dir = 0;
	left_io = -1;
	right_io = -1;
	if (!redir_left_io(&left_io, &node) ||
		!redir_middle(&left_io, &right_io, &dir, &node))
		return ;
	if (!node || !node->lexeme)
		return ;
	if (node->lexeme->set == IO_NAME)
	{
		if (!ft_strncmp(node->lexeme->data, "-", 1))
		{
			add_redir((dir == -1 ? -1 : left_io),
				(dir == -1 ? left_io : -1), list);
			return ;
		}
		else
			right_io = ft_atoi(node->lexeme->data);
	}
	redir_push(left_io, right_io, dir, list);
}
