/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/04 00:36:13 by alkozma           #+#    #+#             */
/*   Updated: 2019/10/09 09:31:37 by alkozma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

/*
** new_node
** Creates a new node with a passed type, lexeme and parent.
** Will add to parent's list of children.
*/

t_node	*new_node(enum e_nodetype set, t_lexeme *lexeme, t_node *parent)
{
	t_node	*new;
	t_node	*tmp;

	new = malloc(sizeof(t_node));
	new->set = set;
	new->lexeme = lexeme;
	new->parent = parent;
	new->next = NULL;
	new->children = NULL;
	if (!parent)
		return (new);
	tmp = parent->children;
	while (tmp && tmp->next)
		tmp = tmp->next;
	if (tmp)
		tmp->next = new;
	else
		parent->children = new;
	return (new);
}

/*
** classify
** Searches input for context and applies a type based on that context.
*/

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

/*
** classify helpers
*/

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

/*
** abstract
** Creates a new empty EXPR node and makes itself the parent of the
** 		passed node.
** Replaces passed node's position in parent's list of children.
*/

t_node	*abstract(t_node *node)
{
	t_node	*parent;
	t_node	*tmp;
	t_node	*child;
	t_node	*new;

	new = malloc(sizeof(t_node));
	new->set = EXPR;
	new->lexeme = NULL;
	if (!node)
	{
		new->next = NULL;
		new->children = NULL;
		new->parent = NULL;
		return (new);
	}
	new->next = node->next;
	new->children = node;
	if (!node->parent)
	{
		node->parent = new;
		new->parent = NULL;
	}
	parent = node->parent;
	tmp = parent->children;
	if (tmp && tmp != node)
	{
		while (tmp->next && tmp->next != node)
			tmp = tmp->next;
		if (tmp)
			tmp->next = new;
	}
	node->next = NULL;
	return (new);
}

void	exec_node_parse(t_node *node)
{
	char	**disp;
	//t_env	*env;
	t_node	*tmp;
	int		sz;
	int		i;

	if (!node)
		return ;
	//env = malloc(sizeof(t_env));
	//ft_printf_fd(STDERR_FILENO, "PARSING\n");
	//make_env(env);
	tmp = node->children;
	disp = NULL;
	sz = 0;
	while (tmp && (tmp->set == ARG || tmp->set == EXEC))
	{
		sz += tmp->lexeme ? 1 : 0;
		tmp = tmp->next;
	}
	tmp = node->children;
	disp = malloc(sizeof(char*) * (sz + 1));
	i = 0;
	while (tmp && (tmp->set == ARG || tmp->set == EXEC))
	{
		if (tmp->lexeme && tmp->lexeme->data)
			disp[i++] = ft_strdup(tmp->lexeme->data);
		tmp = tmp->next;
	}
	disp[i++] = 0;
	//ft_printf_fd(STDERR_FILENO, "%d\n", i);
	run_dispatch(disp, &g_term.env);
	//ft_printf_fd(STDERR_FILENO, "EXECUTED NODE: %s\n", disp[0]);
	free(disp);
}

void	recurse(t_node *head)
{
	t_node	*tmp;
	t_node	*h2;

	h2 = head;
	while (h2)
	{
		//if (h2->lexeme)
		//	ft_printf_fd(STDERR_FILENO, "[%s] ", h2->lexeme->data);
		tmp = h2->children;
		if (tmp)
			recurse(tmp);
		if (tmp && tmp->set == EXEC)
			exec_node_parse(tmp->parent);
		//if (!h2->lexeme || h2->set == MOD)
		//	ft_printf_fd(STDERR_FILENO, "\nv\n");
		h2 = h2->next;
	}
}

void	parser(t_lexeme *lexemes)
{
	t_node	*head;
	enum e_nodetype	classification;

	head = new_node(EXPR, NULL, NULL);
	while (lexemes)
	{
		classification = classify(lexemes);
		if (classification == MOD)
		{
			if (!head->parent)
				head = abstract(head);
			else
				head = head->parent;
		}
		else if (classification == EXEC)
			head = new_node(EXPR, NULL, head);
		new_node(classification, lexemes, head);
		lexemes = lexemes->next;
	}
	while (head->parent)
		head = head->parent;
	recurse(head);
	return ;
}
