/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/04 00:36:13 by alkozma           #+#    #+#             */
/*   Updated: 2019/11/21 20:52:53 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

t_node			*handle_mod(t_node *head, t_lexeme *lexemes, int *invert)
{
	if (*invert)
		head = head->parent ? head->parent : abstract(head);
	if (head->children)
		head = head->parent ? head->parent : abstract(head);
	else
	{
		parse_error(head, lexemes);
		return (NULL);
	}
	*invert = lexemes->set == LESS || lexemes->set == RDLESS ? 1 : 0;
	return (head);
}

t_node			*parser(t_lexeme *lexemes)
{
	t_node	*head;
	enum e_nodetype	classification;
	int		invert;
	int		abs;

	invert = 0;
	abs = 0;
	head = new_node(EXPR, NULL, NULL, invert);
	while (lexemes)
	{
		classification = classify(lexemes);
		if (abs)
		{
			head = head->parent ? head->parent : abstract(head);
			abs = 0;
		}
		if (classification == MOD)
		{
			if (!(head = handle_mod(head, lexemes, &invert)))
				return (NULL);
		}
		else if (classification == EXEC || (classification >= FD_R &&
			classification <= FD_A))
			head = new_node(EXPR, NULL, head, invert);
		else if (classification == ERR)
		{
			parse_error(head, lexemes);
			return (NULL);
		}
		else if (classification == FD_LIT)
		{
			invert = lexemes->next && 
				(lexemes->next->designation == REDIR || lexemes->next->data[0] == '>'
				 || lexemes->next->data[0] == '<') ? 1 : 0;
			if (lexemes->next && (lexemes->next->designation == REDIR ||
						lexemes->next->data[0] == '<' || lexemes->next->data[0] == '>'))
			{
				head = new_node(EXPR, NULL, head, invert);
				lexemes->next->designation = REDIR;
			}
			else
				abs = 1;
		}
		else if (classification == REDIR)
			invert = 0;
		new_node(classification, lexemes, head, invert);
		if (lexemes->set == SEMI)
			head = new_node(EXPR, NULL, head, invert);
		lexemes = lexemes->next;
	}
	while (head->parent)
		head = head->parent;
	return (head);
}
