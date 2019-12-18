/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/04 00:36:13 by alkozma           #+#    #+#             */
/*   Updated: 2019/12/18 09:31:50 by alkozma          ###   ########.fr       */
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

int			count_quotes(char *input)
{
	int	ret;
	int	i;
	int	q;

	ret = 0;
	i = 0;
	q = 0;
	while (*(input + i) && is_operator(input, i) <= 1 && (!ft_isspace(*(input + i)) || q))
	{
		if (*(input + i) == '\'' || *(input + i) == '\"')
		{
			if (q)
				q--;
			else
				q++;
			ret++;
			//q++;
		}
		i++;
	}
	return (ret);
}

char		*lexer_data_assist(char *data, int *op)
{
	char	*ret;
	int		i;
	int		size;
	int		q;
	int		cur;
	int		res;
	int		res2;

	i = 0;
	size = (int)ft_strlen(data);
	if (count_quotes(data) % 2)
	{
		ft_printf_fd(STDERR_FILENO, "-wtsh: quote error\n");
		return (NULL);
	}
	ret = ft_memalloc(size + 1);
	q = 0;
	cur = 0;
	res = 0;
	while (i < size && (q || !ft_isspace(data[i])))
	{
		if ((*op = is_operator(data, i)) > 1)
		{
			ft_strncat(ret, data + i, ft_strlen(g_term.symbls[*op]));
			break ;
		}
		res = data[i] == '\"' || data[i] == '\'' ? (int)data[i] : 0;
		if (data[i] == '\\')
			i++;
		else if (res)
		{
			res2 = (ft_strchr(data + i + 1, data[i]) &&
					(!ft_strchr(data + i + 1, data[i] == '\"' ? '\'' : '\"') ||
					 ft_strchr(data + i + 1, data[i]) >
					 ft_strchr(data + i + 1, data[i] == '\"' ? '\'' : '\"')));
			if (res2)
			{
				q++;
				i++;
			}
			else if (q)
			{
				q--;
				i++;
			}
			else if (!q)
			{
				free(ret);
				ret = NULL;
				ft_printf_fd(STDERR_FILENO, "-wtsh: quote error\n");
				break ;
			}
		}
		if (data[i])
			ret[cur++] = data[i];
		i++;
	}
	return (ret);
}

int			lex_assist(char **input, int *op, t_lexeme **ref)
{
	int	i;
	int	q;
	char	*new_lex_data;

	i = 0;
	q = 0;
	while (**input && ft_isspace(**input))
		(*input)++;
	if (!**input)
		return (0);
	if (!(new_lex_data = lexer_data_assist(*input, op)))
		return (-1);
	new_lex(new_lex_data, *op, ref);
	*input += new_lex_data ? ft_strlen(new_lex_data) + count_quotes(*input) : 0;
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
			if (!ref)
				break ;
		}
	}
	return (ret);
}
