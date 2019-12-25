/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_traverse.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 22:14:35 by calamber          #+#    #+#             */
/*   Updated: 2019/12/20 15:59:04 by alkozma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

static int		exec_node_possible(t_node *node)
{
	t_node	*tmp;

	tmp = node;
	while (tmp)
	{
		if (tmp->set == EXEC || (tmp->set >= FD_R && tmp->set <= FD_A))
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

static void		recurse_empty(t_stats *stats)
{
	if (g_term.children)
		child_wait();
	empty_buffer(stats->f_d);
	stats->f_d[0] = 0;
	stats->f_d[1] = 1;
}

/*
** recurse
** Main function for tree evaluation.
*/

void			recurse(t_node *head, t_stats *stats)
{
	t_node		*tmp;
	t_node		*h2;
	int			main_pipe[2];

	h2 = head;
	while (h2)
	{
		main_pipe[0] = 0;
		main_pipe[1] = 1;
		(tmp = h2->children) ? recurse(tmp, stats) : 0;
		if (h2->lexeme && (h2->lexeme->set == SEMI || h2->lexeme->set == AND))
			recurse_empty(stats);
		if (exec_node_possible(tmp))
		{
			if (find_pipe(tmp->parent) == 1)
				pipe(main_pipe);
			exec_node_parse(tmp->parent, stats->f_d, main_pipe);
			stats->f_d[0] = main_pipe[0];
			stats->f_d[1] = main_pipe[1];
		}
		h2 = h2->next;
	}
}

int				free_arr(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
	arr = NULL;
	return (1);
}

/*
** exec_node_parse
** Executes a given node with children.
*/

void			readfd_subproc(t_node *node, int *in, int *out)
{
	pid_t	pid;

	if ((pid = fork()) == 0)
	{
		if (node->children->set == FD_R)
		{
			readfd(open(node->children->lexeme->data, O_RDONLY), out[1], 0);
			out[1] > 2 ? close(out[1]) : 0;
		}
		else
			exec_handle_redir(node, in, out);
		exit(0);
	}
	child_push(&g_term.children, (int)pid);
	out[1] > 2 ? close(out[1]) : 0;
	in[0] > 2 ? close(in[0]) : 0;
}

void			exec_node_parse(t_node *node, int *in, int *out)
{
	char	**disp;
	t_redir	*redirects;

	redirects = NULL;
	if (!node || node->evaluated)
		return ;
	if (node->children->set >= FD_R && node->children->set <= FD_A)
	{
		ft_printf_fd(STDERR_FILENO, "redir. in0: %d in1: %d\n", in[0], in[1]);
		readfd_subproc(node, in, out);
		return ;
	}
	disp = concat_node(node, &redirects);
	if (run_builtins(disp, &g_term.env) == 2)
		execute_command(in, out, disp, redirects);
	free_arr(disp);
}
