/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/04 00:36:13 by alkozma           #+#    #+#             */
/*   Updated: 2019/10/11 04:25:35 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

int			is_operator(char *op, int pos)
{
	int		i;
	int		ret;
	

	i = 3;
	ret = 0;
	while (g_term.symbls[i])
	{
		//if (pos > 0 && ft_strncmp(op + pos - 1, g_term.symbls[i], ft_strlen(g_term.symbls[i])) == 0)
		//	return (2);
		if (ft_strncmp(op + pos, g_term.symbls[i], ft_strlen(g_term.symbls[i])) == 0)
			return (i);
		i++;
	}
	if (op && ret == 0)
		return (1);
	return (ret);
}

void	lexer_queue_push(t_lexeme **front, t_lexeme **back, char *str, enum e_tokentype set)
{
	t_lexeme *new;


	if (!str)
		return ;
	if (!(new = ft_memalloc(sizeof(t_lexeme))))
		return ;
	if (*back)
		(*back)->next = new;
	else
		*front = new;
	*back = new;
	new->data = str;
	new->set = set;
}

void	parse_expression(t_lexeme **front, t_lexeme **back, char *line, int pos, int i, int len)
{
	//if (ft_isnumber(line + pos, len))
	//	lexer_queue_push(front, back, ft_strndup(line + pos, len), NUMBER);
	//else
	/*
	if (line[i] == '"')
	{
			q = 1;
			pos++;
			while (q == 1 && line[i])
			{
				i++;
				q = line[i] == '"' ? 0 : 1;
			}
			len = i - pos;
			parse_exp(front, back, line, pos, i, len);
			return ;
	}
	if (line[pos] == '"' && line[pos + len - 1] == '"')
		lexer_queue_push(front, back, ft_strndup(line + pos, len), EXPANSION);
	else
	*/
	lexer_queue_push(front, back, ft_strndup(line + pos, len), WORD);
	line_lexer(front, back, line, i);
	return ;
}

/*
void	parse_expansion(t_lexeme **front, t_lexeme **back, char *line, int pos, int i)
{
	int len;
	char ref = line[i];
	while ()
}

int		is_expansion(char c, char ref)
{
	if (ref == 0)
	{
		if (c == '"' || c == '$' || c == '(')
			return (1);
	}
	else
	{
		if (ref == '(' && c == ')')
			return (1);
		else if (ref == '"' && c == '"')
			return (1);
	}
	return (0);
}
*/

void	line_lexer(t_lexeme **front, t_lexeme **back, char *line, int pos)
{
	int i = pos;
	int len;
	int	q;

	q = 0;
	len = 0;
	if (!line || !line[i])
		return ;
	while (line[i])
	{
		int op = 0;
		/*
		if (is_expansion(line[i], 0))
		{
			if (i > pos)
			{
				len = i - pos;
				parse_expression(front, back, line, pos, i, len);
				return ;
			}
			parse_expansion(front, back, line, pos, i);
			return ;
		}
		*/
		if (ft_isspace(line[i]))
		{
			if (i > pos)
			{
				len = i - pos;
				parse_expression(front, back, line, pos, i, len);
				return ;
			}
			i++;
			pos++;
			continue ;
		}
		if ((op = is_operator(line, i)) > 2)
		{
			if (i > pos)
			{
				len = i - pos;
				parse_expression(front, back, line, pos, i, len);
				return ;
			}
			else
			{
				len = ft_strlen(g_term.symbls[op]);
				lexer_queue_push(front, back, ft_strndup(line + i, len), op);
				line_lexer(front, back, line, i + len);
				return ;
			}
		}
		i++;
	}
	if (i > pos)
	{
		len = i - pos;
		parse_expression(front, back, line, pos, i, len);
		return ;
	}
}

t_node	*lexer(char *input)
{
	t_lexeme	*front;
	t_lexeme	*back;

	front = NULL;
	back = NULL;
	if (!input)
		return (NULL);
	line_lexer(&front, &back, input, 0);

	t_lexeme *tmp = front;
	while (tmp)
	{
		ft_printf_fd(STDERR_FILENO, "TYPE: %s, STR: %s\n", g_term.symbls[tmp->set], tmp->data);
		tmp = tmp->next;
	}
	return(parser(front));
}
