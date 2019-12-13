/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/04 00:36:13 by alkozma           #+#    #+#             */
/*   Updated: 2019/12/13 14:28:34 by alkozma          ###   ########.fr       */
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

int			lexer_helper(char *input, int i, int *op)
{
	int		tmp;

	while (*(input + i) && !ft_isspace(*(input + i)))
	{
		if ((*(input + i) == '\'' || *(input + i) == '\"') && (i - 1 && ft_isspace(*(input + i - 1))))
			i += handle_quote(input + i);
		*op = is_operator(input, i);
		if (*op > 1)
			break ;
		if (*(input + i) == '\\')
		{
			tmp = i;
			while (input[i])
			{
				input[i] = input[i + 1];
				i++;
			}
			i = tmp;
		}
		i++;
	}
	return (i);
}

t_node		*lexer(char *input)
{
	t_lexeme	*ref;
	int			i;
	int			op;
	t_node		*ret;
	t_lexeme	*tmp;
	char		*buff;

	ref = NULL;
	op = 0;
	ret = NULL;
	tmp = NULL;
	buff = ft_strdup(input);
	i = 0;
	while (buff[i])
	{
		while (buff[i] && ft_isspace(buff[i]))
			i++;
		if (!buff[i])
			break ;
		i = lexer_helper(buff, i, &op);
		i ? new_lex(ft_strndup(buff, i), 1, &ref) : 0;
		if (op > 1)
			buff = add_lex_op(&ref, buff, op);
	}
	if (!(ret = parser(ref)))
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
	buff ? free(buff) : 0;
	return (ret);
}
