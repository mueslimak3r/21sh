/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_op_handler.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/18 19:12:05 by calamber          #+#    #+#             */
/*   Updated: 2019/12/13 14:28:35 by alkozma          ###   ########.fr       */
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

int			handle_quote_helper(char *input, int *i)
{
	int	ret;
	int	d;

	d = 0;
	input[*i] = input[*i + 2];
	ret = *i;
	while (input[ret + 1])
	{
		input[ret + 1] = input[ret + 2];
		ret++;
		d++;
	}
	input[ret] = 0;
	*i = ret;
	ret -= d;
	return (ft_isspace(input[ret]) ? ret - 1 : ret);
}

int			handle_quote(char *input)
{
	int	i;
	int match_single;
	int	match_double;

	i = 0;
	match_single = -1;
	match_double = -1;
	while (input[i])
	{
		if ((match_single == 1 && input[i + 1] == '\'') ||
			(match_double == 1 && input[i + 1] == '\"'))
			handle_quote_helper(input, &i);
		match_single = input[i] == '\'' ? -match_single : match_single;
		match_double = input[i] == '\"' ? -match_double : match_double;
		if (match_single == 1 && match_double == 1)
			return (-1);
		if (match_single == 1 || match_double == 1)
			input[i] = input[i + 1];
		i++;
	}
	return (1);
}
