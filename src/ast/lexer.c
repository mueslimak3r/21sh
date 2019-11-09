/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/04 00:36:13 by alkozma           #+#    #+#             */
/*   Updated: 2019/10/25 03:33:40 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

/*
** #define LEX_DEBUG
*/

int			is_operator(char *op, int pos)
{
	int		i;
	int		ret;

	i = 3;
	ret = 0;
	while (g_term.symbls[i])
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

t_lexeme	*new_lex_helper(t_lexeme **head, t_lexeme *new)
{
	t_lexeme	*tmp;

	tmp = *head;
	if (!tmp)
		return ((*head = new));
	while (tmp)
	{
		if (!tmp->next)
		{
			tmp->next = new;
			return (tmp);
		}
		tmp = tmp->next;
	}
	return (tmp);
}

t_lexeme	*new_lex(char *data, enum e_tokentype type, t_lexeme **head)
{
	t_lexeme	*new;
	t_lexeme	*tmp;
	char		*env;

	new = malloc(sizeof(t_lexeme));
	new->set = type;
	new->data = data;
	new->pos = 0;
	new->designation = BASE;
	new->next = NULL;
	if (data[0] == '$' && data[1] != '(')
	{
		env = find_env(data + 1);
		new->data = env ? ft_strdup(env) : NULL;
		free(data);
	}
	return (new_lex_helper(head, new));
}

int			handle_quote(char *input)
{
	int	i;
	int match_single;
	int	match_double;
	int	ret;

	i = 0;
	match_single = -1;
	match_double = -1;
	while (input[i])
	{
		if ((match_single == 1 && input[i + 1] == '\'') ||
			(match_double == 1 && input[i + 1] == '\"'))
		{
			input[i] = input[i + 2];
			ret = i;
			while (input[i + 1])
			{
				input[i + 1] = input[i + 2];
				i++;
			}
			input[i] = 0;
			return (ft_isspace(input[ret]) ? ret - 1 : ret);
		}
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

t_node		*lexer(char *input)
{
	t_lexeme	*ref;
	int			i;
	int			op;
	int			tmp;

	ref = NULL;
	op = 0;
	while (*input)
	{
		i = 0;
		while (*input && ft_isspace(*input))
			input++;
		while (*(input + i) && !ft_isspace(*(input + i)))
		{
			if (*(input + i) == '\'' || *(input + i) == '\"')
				i += handle_quote(input + i);
			op = is_operator(input, i);
			if (op > 1)
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
		i ? new_lex(ft_strndup(input, i), 1, &ref) : 0;
		input += i;
		if (op > 1)
		{
			new_lex(ft_strndup(input, ft_strlen(g_term.symbls[op])), op, &ref);
			input += ft_strlen(g_term.symbls[op]);
		}
	}
	return (parser(ref));
}
