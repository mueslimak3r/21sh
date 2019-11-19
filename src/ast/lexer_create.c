/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_create.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/18 19:11:19 by calamber          #+#    #+#             */
/*   Updated: 2019/11/18 19:11:20 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

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

char		*add_lex_op(t_lexeme **head, char *line, int op)
{
	char *io_name;

	if (op < L_REDIRECT)
	{
		new_lex(ft_strndup(line, ft_strlen(g_term.symbls[op])), op, head);
		line += ft_strlen(g_term.symbls[op]);
	}
	else if (op == L_REDIRECT)
		line += handle_redirect(line, head);
	else if (op == IO_NAME)
	{
		io_name = ft_itoa(ft_atoi(line));
		new_lex(io_name, op, head);
		line += ft_strlen(io_name);
	}
	return (line);
}
