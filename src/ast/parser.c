/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/04 00:36:13 by alkozma           #+#    #+#             */
/*   Updated: 2019/12/20 15:40:14 by alkozma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

t_node	*handle_mod(t_node *head, t_lexeme *lexemes, int *inv)
{
	if (*inv)
		head = head->parent ? head->parent : abstract(head);
	if (head->children)
		head = head->parent ? head->parent : abstract(head);
	else
	{
		parse_error(head, lexemes);
		return (NULL);
	}
	*inv = (lexemes->set == LESS || lexemes->set == RDLESS) ? 1 : 0;
	return (head);
}

t_node	*handle_fd_lit(t_node *head, t_lexeme *lexemes, int *inv, int *abs)
{
	*inv = (lexemes->next && (lexemes->next->designation == REDIR ||
	lexemes->next->data[0] == '>' || lexemes->next->data[0] == '<')) ? 1 : 0;
	if (lexemes->next && (lexemes->next->designation == REDIR ||
		lexemes->next->data[0] == '<' || lexemes->next->data[0] == '>'))
	{
		head = new_node(EXPR, NULL, head, *inv);
		lexemes->next->designation = REDIR;
	}
	else
		*abs = 1;
	return (head);
}

t_node	*parser_type_handler(t_node *head, t_lexeme *lexemes,
	int *inv, int *abs)
{
	enum e_nodetype classification;

	classification = classify(lexemes);
	if (classification == MOD)
	{
		if (!(head = handle_mod(head, lexemes, inv)))
			return (NULL);
	}
	else if (classification == EXEC || (classification >= FD_R &&
		classification <= FD_A))
		head = new_node(EXPR, NULL, head, *inv);
	else if (classification == ERR)
	{
		parse_error(head, lexemes);
		return (NULL);
	}
	else if (classification == FD_LIT)
		head = handle_fd_lit(head, lexemes, inv, abs);
	else if (classification == REDIR)
		*inv = 0;
	return (head);
}

/*
** int mod uses bit flags 0x1 for abstraction and 0x2 for inversion
*/

t_node	*parser(t_lexeme *lexemes)
{
	t_node			*head;
	enum e_nodetype	classification;
	int				inv;
	int				abs;

	inv = 0;
	abs = 0;
	head = new_node(EXPR, NULL, NULL, inv);
	while (lexemes)
	{
		classification = classify(lexemes);
		if (abs && ((head = head->parent ? head->parent
							: abstract(head)) || !head))
			abs = 0;
		if (!(head = parser_type_handler(head, lexemes, &inv, &abs)))
			return (NULL);
		new_node(classification, lexemes, head, inv);
		if (lexemes->set == SEMI)
			head = new_node(EXPR, NULL, head, inv);
		lexemes && lexemes->data ? ft_setenv("_", lexemes->data) : 0;
		lexemes = lexemes->next;
	}
	while (head->parent)
		head = head->parent;
	return (head);
}
