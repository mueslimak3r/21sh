/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/04 00:36:13 by alkozma           #+#    #+#             */
/*   Updated: 2019/12/11 10:28:09 by calamber         ###   ########.fr       */
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
	int	tmp;

	while (*(input + i) && !ft_isspace(*(input + i)))
	{
		if (*(input + i) == '\'' || *(input + i) == '\"')
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

	ref = NULL;
	op = 0;
	while (*input)
	{
		i = 0;
		while (*input && ft_isspace(*input))
			input++;
		if (!*input)
			break ;
		i = lexer_helper(input, i, &op);
		i ? new_lex(ft_strndup(input, i), 1, &ref) : 0;
		input += i;
		if (op > 1)
			input = add_lex_op(&ref, input, op);
	}
	//print_lex(ref);
	return (parser(ref));
}
