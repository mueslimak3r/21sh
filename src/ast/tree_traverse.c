/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_traverse.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 22:14:35 by calamber          #+#    #+#             */
/*   Updated: 2019/12/09 17:42:19 by calamber         ###   ########.fr       */
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
	if (g_term.pid > -1)
		waitpid(g_term.pid, 0, 0);
	empty_buffer(stats->f_d);
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
	static int	pipes;

	h2 = head;
	while (h2)
	{
		main_pipe[0] = 0;
		main_pipe[1] = 1;
		pipes += (count_pipes(h2->children));
		(tmp = h2->children) ? recurse(tmp, stats) : 0;
		if (h2->lexeme && (h2->lexeme->set == SEMI || h2->lexeme->set == AND))
			recurse_empty(stats);
		if (exec_node_possible(tmp))
		{
			pipes ? pipe(main_pipe) : 0;
			exec_node_parse(tmp->parent, stats->f_d, main_pipe);
			//pipes ? close(main_pipe[1]) : 0;
			pipes -= pipes ? 1 : 0;
			stats->f_d[0] = main_pipe[0];
			stats->f_d[1]= main_pipe[1];
		}
		h2 = h2->next;
	}
}

static void		free_arr(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
	arr = NULL;
}

/*
** exec_node_parse
** Executes a given node with children.
*/

void			exec_heredoc(t_node *node, int *out)
{
	t_tbuff	*hdbuff;
	t_tbuff *curr;
	char	*instr;
	char	*tmp;

	hdbuff = NULL;
	instr = NULL;
	tmp = NULL;
	curr = g_term.buff;
	while (!hdbuff || ft_strcmp(node->children->lexeme->data, hdbuff->buff_str))
	{
		tbuff_new(&hdbuff);
		ft_readstdin_line(hdbuff, 1);
		if (hdbuff && hdbuff->buff_str && ft_strcmp(node->children->lexeme->data, hdbuff->buff_str))
		{
			hdbuff->buff_str = ft_strjoin(hdbuff->buff_str, "\n");
			if (instr)
			{
				tmp = ft_strjoin(instr, hdbuff->buff_str);
				free(instr);
			}
			else
				tmp = ft_strdup(hdbuff->buff_str);
			instr = ft_strdup(tmp);
			free(tmp);
			tmp = NULL;
		}
	}
	write(out[1], instr, ft_strlen(instr));
	if (instr)
		free(instr);
	instr = NULL;
	//ft_readstdin_line(g_term.hd_buff, 1, node->children->lexeme->data);
	// print to out[1]
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
		if (node->children->set == FD_R)
			readfd(open(node->children->lexeme->data, O_RDONLY), out[1], 0);
		else if (node->children->set == FD_W)
			readfd(in[0], open(node->children->lexeme->data,
				O_WRONLY | O_CREAT | O_TRUNC, 0644), 1);
		else if (node->children->set == FD_A)
			readfd(in[0], open(node->children->lexeme->data,
				O_WRONLY | O_CREAT | O_APPEND, 0644), 1);
		else
		{
			exec_heredoc(node, out);
		}
		return ;
	}
	disp = concat_node(node, &redirects);
	if (run_builtins(disp, &g_term.env) == 2)
		execute_command(in, out, disp, redirects);
	free_arr(disp);
}
