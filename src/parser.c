/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkozma <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/04 00:36:13 by alkozma           #+#    #+#             */
/*   Updated: 2019/10/07 09:25:52 by alkozma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

t_node	*new_node(enum e_nodetype set, t_lexeme *lexeme, t_node *parent)
{
	t_node	*new;

	new = malloc(sizeof(t_node));
	new->set = set;
	new->lexeme = lexeme;
	new->parent = parent;
	new->next = NULL;
	new->children = NULL;
	return (new);
}

void	add_child(t_node **parent, t_node *child)
{
	t_node	*tmp;
	t_node	*tmpc;

	tmp = *parent;
	tmpc = tmp->children;
	while (tmpc && tmpc->next)
		tmpc = tmpc->next;
	if (tmpc)
		tmpc->next = child;
	else
		*parent = child;
}

enum e_nodetype	classify(t_lexeme *lexeme)
{
	if (!lexeme)
		return (1);
	if (lexeme->designation != BASE)
		return (lexeme->designation);
	if (is_exec(lexeme))
		return (EXEC);
	else if (is_arg(lexeme))
		return (ARG);
	else if (is_mod(lexeme))
		return (MOD);
	return (2);
}

int	is_mod(t_lexeme *lexeme)
{
	if (!lexeme)
		return (0);
	if (lexeme->set >= AND && lexeme->set <= IO_NUMBER)
		return (1);
	return (0);
}

int	is_arg(t_lexeme *lexeme)
{
	enum e_nodetype	tmp;

	if (!lexeme)
		return (0);
	if (lexeme->set == WORD &&
			(!lexeme->next || (is_arg(lexeme->next) || is_mod(lexeme->next))))
	{
		if (is_arg(lexeme->next))
			lexeme->next->designation = ARG;
		else if (is_mod(lexeme->next))
			lexeme->next->designation = MOD;
		return (1);
	}
	return (0);
}

int	is_exec(t_lexeme *lexeme)
{
	if (!lexeme)
		return (0);
	if (lexeme->set == WORD && 
			(!lexeme->next || (is_arg(lexeme->next) || is_mod(lexeme->next))))
	{
		if (is_arg(lexeme->next))
			lexeme->next->designation = ARG;
		else if (is_mod(lexeme->next))
			lexeme->next->designation = MOD;
		return (1);
	}
	return (0);
}

void	plant_tree(t_lexeme *lexemes)
{
	t_node	*head;
	t_node	*tmp;

	head = new_node(EXPR, NULL, NULL);
	while (lexemes)
	{
		tmp = new_node(classify(lexemes), lexemes, head);
		if (tmp->set == EXPR)
			ft_printf_fd(STDERR_FILENO, "EXPR ");
		else if (tmp->set == EXEC)
			ft_printf_fd(STDERR_FILENO, "EXEC ");
		else if (tmp->set == ARG)
			ft_printf_fd(STDERR_FILENO, "ARG ");
		else if (tmp->set == MOD)
			ft_printf_fd(STDERR_FILENO, "MOD ");
		else
			ft_printf_fd(STDERR_FILENO, "ERR ");
		lexemes = lexemes->next;
	}
	/*
	while (head)
	{
		t_node *t = head;
		head = head->next;
		free(t);
	}
	*/
	ft_printf_fd(STDERR_FILENO, "\n");
	return ;
}

void	lexer(void)
{
	t_lexeme	*test;

	test = malloc(sizeof(t_lexeme));
	test->data = "echo";
	test->set = WORD;
	test->pos = 0;
	test->designation = BASE;
	test->next = malloc(sizeof(t_lexeme));
	test->next->data = "test";
	test->next->set = WORD;
	test->next->pos = 0;
	test->next->designation = BASE;
	test->next->next = malloc(sizeof(t_lexeme));
	test->next->next->data = "|";
	test->next->next->set = PIPE;
	test->next->next->pos = 0;
	test->next->next->designation = BASE;
	test->next->next->next = malloc(sizeof(t_lexeme));
	test->next->next->next->data = "cat";
	test->next->next->next->set = WORD;
	test->next->next->next->pos = 0;
	test->next->next->next->designation = BASE;
	test->next->next->next->next = malloc(sizeof(t_lexeme));
	test->next->next->next->next->data = "-e";
	test->next->next->next->next->set = WORD;
	test->next->next->next->next->pos = 0;
	test->next->next->next->next->designation = BASE;
	test->next->next->next->next->next = malloc(sizeof(t_lexeme));
	test->next->next->next->next->next->data = "-e";
	test->next->next->next->next->next->set = WORD;
	test->next->next->next->next->next->pos = 0;
	test->next->next->next->next->next->designation = BASE;
	test->next->next->next->next->next->next = NULL;

	plant_tree(test);
	/*
	while (test)
	{
		t_lexeme *tmp = test;
		test = test->next;
		free (tmp);
	}
	*/
	ft_printf_fd(STDERR_FILENO, "i tried\n");
}
