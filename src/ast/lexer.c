/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/04 00:36:13 by alkozma           #+#    #+#             */
/*   Updated: 2019/12/19 16:24:21 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

/*
** #define LEX_DEBUG
*/

void		print_lex(t_lexeme *head)
{
	ft_printf_fd(STDERR_FILENO, "LEXEMES: ");
	while (head)
	{
		ft_printf_fd(STDERR_FILENO,
			"  |type: %d, str: %s|  ", head->set, head->data);
		head = head->next;
	}
	ft_printf_fd(STDERR_FILENO, "\n");
}

int			lexer_data_assist2(char *data, char **ret, int *i, int *cur)
{
	int	size;

	size = (int)ft_strlen(data);
	if (data[*i] == '\\')
	{
		if (*i < size - 1 && data[*i + 1])
			(*ret)[(*cur)++] = data[(*i)++ + 1];
		(*i)++;
	}
	else if (*i < size && (data[*i] == '\"' || data[*i] == '\''))
	{
		if (!quote_parser(data, ret, i, cur))
			return (0);
	}
	else if (data[*i])
		(*ret)[(*cur)++] = data[(*i)++];
	return (1);
}

char		*lexer_data_assist(char *data, int *op, int *amt)
{
	char	*ret;
	int		i;
	int		size;
	int		cur;

	i = 0;
	size = (int)ft_strlen(data);
	ret = ft_memalloc(size + 1);
	cur = 0;
	while (i < size && !ft_isspace(data[i]))
	{
		if ((*op = is_operator(data, i)) > 1)
		{
			(i > 0) ? *op = 1 : ft_memdel((void**)&ret);
			break ;
		}
		if (!lexer_data_assist2(data, &ret, &i, &cur))
			break ;
	}
	ret ? *amt = i - cur : 0;
	return (ret);
}

int			lex_assist(char **input, int *op, t_lexeme **ref)
{
	int		i;
	int		q;
	int		amt;
	char	*new_lex_data;

	i = 0;
	q = 0;
	amt = 0;
	while (**input && ft_isspace(**input))
		(*input)++;
	if (!**input)
		return (0);
	if (!(new_lex_data = lexer_data_assist(*input, op, &amt)) && *op <= 1)
		return (-1);
	if (*op <= 1)
	{
		*input += ft_strlen(new_lex_data) + amt;
		new_lex(new_lex_data, *op, ref);
	}
	else
		*input = add_lex_op(ref, *input, *op);
	return (1);
}

t_node		*lexer(char *input)
{
	t_lexeme	*ref;
	int			op;
	t_node		*ret;
	t_lexeme	*tmp;
	int			retu;

	ref = NULL;
	op = 0;
	ret = NULL;
	tmp = NULL;
	retu = 0;
	while (*input)
		if (!(retu = lex_assist(&input, &op, &ref)) || retu < 0)
			break ;
	if (retu < 0 || !(ret = parser(ref)))
	{
		while ((tmp = ref))
		{
			ref = ref->next;
			if (tmp->data)
				free(tmp->data);
			free(tmp);
		}
	}
	return (ret);
}
