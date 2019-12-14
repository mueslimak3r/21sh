/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/13 19:30:06 by calamber          #+#    #+#             */
/*   Updated: 2019/12/13 19:54:20 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

bool	exec_heredoc_helper(t_tbuff *hdbuff, t_node *node,
									int ret, char **instr)
{
	if (ret > 0 && hdbuff && hdbuff->buff_str &&
		ft_strcmp(node->children->lexeme->data, hdbuff->buff_str))
	{
		hdbuff->buff_str = ft_strjoin_free(hdbuff->buff_str, "\n", 0);
		if (*instr)
			*instr = ft_strjoin_free(*instr, hdbuff->buff_str, 0);
		else
			*instr = ft_strdup(hdbuff->buff_str);
	}
	else if (ret == 0)
	{
		if (*instr)
			*instr = ft_strjoin_free(*instr, "\n", 0);
		else
			*instr = ft_strdup("\n");
	}
	else if (ret > 0 && hdbuff && hdbuff->buff_str &&
		!ft_strcmp(node->children->lexeme->data, hdbuff->buff_str))
		return (true);
	return (false);
}

void	exec_heredoc(t_node *node, int *in, int *out)
{
	t_tbuff	*hdbuff;
	char	*instr;
	int		ret;
	bool	found_end;

	found_end = false;
	instr = NULL;
	while (!found_end)
	{
		hdbuff = g_term.buff;
		tbuff_new(&hdbuff);
		if ((ret = ft_readstdin_line(&hdbuff, 1)) < 0)
			break ;
		found_end = exec_heredoc_helper(hdbuff, node, ret, &instr);
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
	in[0] > 2 ? close(in[0]) : 0;
}
