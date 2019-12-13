/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_traverse.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 22:14:35 by calamber          #+#    #+#             */
/*   Updated: 2019/12/13 06:10:18 by calamber         ###   ########.fr       */
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
	stats->f_d[0] = 0;
	stats->f_d[1] = 1;
}

int             find_pipe_r(t_node *head)
{
    t_node *tmp;
    int     ret;
    
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
int             find_pipe(t_node *head)
{
    t_node      *tmp;
    int         ret;
    if (!head)
        return (0);
    if ((ret = find_pipe_r(head)) != 0)
        return (ret);
    if (head->parent)
    {
        tmp = head->parent->next;
        if (tmp &&  (ret = find_pipe(tmp)) != 0)
            return (ret);
    }
    return (0);
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
			{
				ft_printf_fd(STDERR_FILENO, "piped\n");
				pipe(main_pipe);
			}
			exec_node_parse(tmp->parent, stats->f_d, main_pipe);
			//pipes ? close(main_pipe[1]) : 0;
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

char	*ft_strjoin_free(char *s1, char *s2, int free_which)
{
	int		i;
	int		j;
	char	*str;

	if (!s1 && !s2)
		return (NULL);
	i = 0;
	j = 0;
	str = (char *)malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (str == NULL)
		return (NULL);
	while (s1 && s1[i] != '\0')
	{
		str[i] = s1[i];
		i++;
	}
	while (s2 && s2[j] != '\0')
	{
		str[i + j] = s2[j];
		j++;
	}
	str[i + j] = '\0';
	free(free_which ? s2 : s1);
	return (str);
}

/*
** exec_node_parse
** Executes a given node with children.
*/

void			exec_heredoc(t_node *node, int *out)
{
	t_tbuff	*hdbuff;
	char	*instr;
	int		ret;
	bool	found_end;

	found_end = false;
	hdbuff = g_term.buff;
	instr = NULL;
	while (!found_end)
	{
		hdbuff = g_term.buff;
		tbuff_new(&hdbuff);
		if ((ret = ft_readstdin_line(&hdbuff, 1)) < 0)
			break ;
		if (ret > 0 && hdbuff && hdbuff->buff_str && ft_strcmp(node->children->lexeme->data, hdbuff->buff_str))
		{
			hdbuff->buff_str = ft_strjoin_free(hdbuff->buff_str, "\n", 0);
			if (instr)
				instr = ft_strjoin_free(instr, hdbuff->buff_str, 0);
			else
				instr = ft_strdup(hdbuff->buff_str);
		}
		else if (ret == 0)
		{
			if (instr)
				instr = ft_strjoin_free(instr, "\n", 0);
			else
				instr = ft_strdup("\n");
		}
		else if (ret > 0 && hdbuff && hdbuff->buff_str && !ft_strcmp(node->children->lexeme->data, hdbuff->buff_str))
			found_end = true;
		if (ret > 0)
			tbuff_rm_edits(&hdbuff);
		g_term.buff = hdbuff;
	}
	if (instr)
	{
		if (!g_term.sigs.sigint)
			write(out[1], instr, ft_strlen(instr));
		free(instr);
	}
	g_term.buff = hdbuff;
	g_term.sigs.sigint = false;
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
	ft_printf_fd(STDERR_FILENO, "in0: %d in1: %d out0: %d out1: %d\n", in[0], in[1], out[0], out[1]);
	if (node->children->set >= FD_R && node->children->set <= FD_A)
	{
		if (node->children->set == FD_R)
		{
			readfd(open(node->children->lexeme->data, O_RDONLY), out[1], 0);
			out[1] > 2 ? close(out[1]) : 0;
		}
		else if (node->children->set == FD_W)
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
			exec_heredoc(node, out);
			in[0] > 2 ? close(in[0]) : 0;
			out[1] > 2 ? close(out[1]) : 0;
		}
		return ;
	}
	disp = concat_node(node, &redirects);
	if (run_builtins(disp, &g_term.env) == 2)
		execute_command(in, out, disp, redirects);
	free_arr(disp);
}
