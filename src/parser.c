/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/04 00:36:13 by alkozma           #+#    #+#             */
/*   Updated: 2019/10/11 14:05:43 by alkozma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"
//#define TREE_DEBUG
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
	new->evaluated = 0;
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
	int		res;

	if (!lexeme)
		return (1);
	if (lexeme->designation != BASE)
		return (lexeme->designation);
	if ((res = is_exec(lexeme)))
		return (res > 0 ? EXEC : ERR);
	else if ((res = is_arg(lexeme)))
		return (res > 0 ? ARG : ERR);
	else if ((res = is_mod(lexeme)))
		return (res > 0 ? MOD : ERR);
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
	{
		if (lexeme->next && !is_mod(lexeme->next))
			return (1);
		return (-1);
	}
	return (0);
}

int	is_arg(t_lexeme *lexeme)
{
	enum e_nodetype	tmp;

	if (!lexeme)
		return (0);
	if (lexeme->set == WORD &&
			(!lexeme->next || (is_arg(lexeme->next) || is_mod(lexeme->next) > 0)))
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
			(!lexeme->next || (is_arg(lexeme->next) || is_mod(lexeme->next) > 0)))
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

/*
** concat_node
** Given a node, returns a string array of the data of the children's lexemes.
*/

char	**concat_node(t_node *node)
{
	char	**ret;
	t_node	*tmp;
	int		sz;
	int		i;

	if (!node)
		return (NULL);
	tmp = node->children;
	ret = NULL;
	sz = 0;
	while (tmp && (tmp->set == ARG || tmp->set == EXEC))
	{
		sz += tmp->lexeme ? 1 : 0;
		tmp = tmp->next;
	}
	tmp = node->children;
	ret = malloc(sizeof(char*) * (sz + 1));
	i = 0;
	while (tmp && (tmp->set == ARG || tmp->set == EXEC))
	{
		if (tmp->lexeme && tmp->lexeme->data)
			ret[i++] = ft_strdup(tmp->lexeme->data);
		tmp = tmp->next;
	}
	ret[i] = 0;
	return (ret);
}

/*
** exec_node_parse
** Executes a given node with children.
*/

void	exec_node_parse(t_node *node)
{
	char	**disp;
	t_node	*tmp;

	if (!node || node->evaluated)
		return ;
	disp = concat_node(node);
	if (node->next && node->next->lexeme->set == PIPE)
	{
		exec_pipe(node, node->next->next);
		node->evaluated = 1;
		tmp = node;
		while (tmp->next && tmp->next->lexeme && tmp->next->lexeme->set == PIPE)
		{
			tmp = tmp->next->next;
			tmp->evaluated = 1;
		}
	}
	else if (run_builtins(disp, &g_term.env) == 2)
		run_dispatch(disp, &g_term.env);
	free(disp);
}

/*
** clean_tree
** Recursively frees the tree and associated lexemes.
*/

void	clean_tree(t_node *head)
{
	t_node	*tmp;
	t_node	*tmp2;
	t_node	*h2;

	h2 = head;
	while (h2)
	{
		tmp = h2->children;
		if (tmp)
			clean_tree(tmp);
		tmp2 = h2;
		h2 = h2->next;
		if (tmp2->lexeme)
		{
			free(tmp2->lexeme->data);
			free(tmp2->lexeme);
			tmp2->lexeme = NULL;
		}
		free(tmp2);
		tmp2 = NULL;
	}
}

/*
** recurse
** Main function for tree evaluation.
*/

void	recurse(t_node *head)
{
#ifdef TREE_DEBUG
	static int st = 0;
#endif
	t_node	*tmp;
	t_node	*h2;

	h2 = head;
#ifdef TREE_DEBUG
	st++;
#endif
	while (h2)
	{
		tmp = h2->children;
#ifdef TREE_DEBUG
		if (h2 && h2->lexeme) {
			write(STDERR_FILENO, "            ", st * 2);
			ft_printf_fd(STDERR_FILENO, "[IN TREE || TYPE: %s, STR: %s]\n", g_term.symbls[h2->lexeme->set], h2->lexeme->data);
		} else if (h2) {
			write(STDERR_FILENO, "            ", st * 2);
			ft_printf_fd(STDERR_FILENO, "[IN TREE || TYPE: parent, STR: N/A]\n");
		}
#endif
		if (tmp)
			recurse(tmp);
		if (tmp && tmp->set == EXEC)
			exec_node_parse(tmp->parent);
		h2 = h2->next;
	}
#ifdef TREE_DEBUG
	st--;
#endif
}

t_node	*parser(t_lexeme *lexemes)
{
	t_node	*head;
	enum e_nodetype	classification;

	head = new_node(EXPR, NULL, NULL);
	while (lexemes)
	{
		classification = classify(lexemes);
		if (classification == MOD)
		{
			if (head->children)
				head = head->parent ? head->parent : abstract(head);
			else
			{
				parse_error(head, lexemes);
				return (NULL);
			}
		}
		else if (classification == EXEC)
			head = new_node(EXPR, NULL, head);
		else if (classification == ERR)
		{
			while (head && head->parent)
				head = head->parent;
			parse_error(head, lexemes);
			return (NULL);
		}
		new_node(classification, lexemes, head);
		lexemes = lexemes->next;
	}
	while (head->parent)
		head = head->parent;
	return (head);
}
#undef TREE_DEBUG
