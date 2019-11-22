#include "ftshell.h"

void	add_redir(int src, int dst, t_redir **list)
{
	t_redir *tmp;

	tmp = malloc(sizeof(*tmp));
	tmp->src = src;
	tmp->dst = dst;
	tmp->next = *list;
	*list = tmp;
}

void				redir_pipes(t_node *node, t_redir **list)
{
	int		left_io = -1;
	int		right_io = -1;
	int		dir = 0;

	t_node	*tmp;
	tmp = node->children;
	if (!tmp || !tmp->lexeme)
		return ;
	if (tmp->lexeme->set == IO_NAME)
	{
		left_io = ft_atoi(tmp->lexeme->data);
		tmp = tmp->next;
	}
	if (!tmp || !tmp->lexeme)
		return ;
	if (tmp->lexeme->set == L_REDIRECT || tmp->lexeme->set == R_REDIRECT)
	{
		dir = tmp->lexeme->set == L_REDIRECT ? -1 : 1;
		if (ft_strchr(tmp->lexeme->data, '&') && ft_strchr(tmp->lexeme->data, '&') < ft_strchr(tmp->lexeme->data, (tmp->lexeme->set == L_REDIRECT ? '<' : '>')))
		{
			if (left_io == -1)
				left_io = -2;
			else
			{
				ft_printf_fd(STDERR_FILENO, "redir syntax err\n");
				return ;
			}
		}
		else if (ft_strchr(tmp->lexeme->data, '&'))
		{
			right_io = -2;
		}
		if (left_io == -1)
			left_io = 1;
		tmp = tmp->next;
	}
	else
	{
		ft_printf_fd(STDERR_FILENO, "redir syntax err\n");
		return ;
	}
	if (!tmp || !tmp->lexeme)
		return ;
	if (tmp->lexeme->set == IO_NAME)
	{
		if (!ft_strncmp(tmp->lexeme->data, "-", 1))
		{
			add_redir((dir == -1 ? -1 : left_io), (dir == -1 ? left_io : -1), list);
			return ;
		}
		else
			right_io = ft_atoi(tmp->lexeme->data);
	}
	if (dir == 0 || left_io == -1 || right_io == -1 || (left_io == -2 && right_io == -2))
	{
		ft_printf_fd(STDERR_FILENO, "redir syntax err\n");
		return ;
	}
	ft_printf("[%d]->[%d]\n", (dir == -1 ? right_io : left_io), (dir == -1 ? left_io : right_io));
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
		add_redir((dir == -1 ? right_io : left_io), (dir == -1 ? left_io : right_io), list);
}