/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/04 00:36:13 by alkozma           #+#    #+#             */
/*   Updated: 2019/10/07 18:44:15 by calamber         ###   ########.fr       */
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
	ft_printf_fd(STDERR_FILENO, "PARSING\n");
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
	ft_printf_fd(STDERR_FILENO, "%d\n", i);
	run_dispatch(disp, &g_term.env);
	ft_printf_fd(STDERR_FILENO, "EXECUTED NODE: %s\n", disp[0]);
	free(disp);
}

void	parser(t_node *head)
{
	t_node	*tmp;
	t_node	*h2;

	h2 = head;
	while (h2)
	{
		if (h2->lexeme)
			ft_printf_fd(STDERR_FILENO, "[%s] ", h2->lexeme->data);
		tmp = h2->children;
		if (tmp)
			parser(tmp);
		if (tmp && tmp->set == EXEC)
			exec_node_parse(tmp->parent);
		if (!h2->lexeme || h2->set == MOD)
			ft_printf_fd(STDERR_FILENO, "\nv\n");
		h2 = h2->next;
	}
}

void	plant_tree(t_lexeme *lexemes)
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
	parser(head);

	/*
	while (head->children)
	{
		ft_printf_fd(STDERR_FILENO, "[%s]", head->children->lexeme ? head->children->lexeme->data : "NULL");
		while (head->children->children)
		{
			ft_printf_fd(STDERR_FILENO, "(%s)", head->children->children->lexeme ? head->children->children->lexeme->data : "NULL");
			head->children->children = head->children->children->next;
		}
		head->children = head->children->next;
	}
	*/
	return ;
}

void	lexer_queue_push(t_lexeme **front, t_lexeme **back, char *str, enum e_tokentype set)
{
	t_lexeme *new;


	if (!str)
		return ;
	if (!(new = ft_memalloc(sizeof(t_lexeme))))
		return ;
	if (*back)
		(*back)->next = new;
	else
		*front = new;
	*back = new;
	new->data = str;
	new->set = set;
}

void	line_lexer(t_lexeme **front, t_lexeme **back, char *line, int pos)
{
	int i = pos;
	int len;

	if (!line)
		return ;
	if (!line[i])
		return ;
	while (line[i])
	{
		int op = 0;
		if (ft_isspace(line[i]))
		{
			if (i > pos)
			{
				len = i - pos;
				lexer_queue_push(front, back, ft_strndup(line + pos, len), WORD);
				line_lexer(front, back, line, i);
				return ;
			}
			i++;
			pos++;
			continue ;
		}
		if ((op = is_operator(line, i)) > 1)
		{
			if (i > pos)
			{
				len = i - pos;
				lexer_queue_push(front, back, ft_strndup(line + pos, len), WORD);
				line_lexer(front, back, line, i);
				return ;
			}
			else
			{
				len = ft_strlen(g_term.symbls[op]);
				lexer_queue_push(front, back, ft_strndup(line + i, len), op);
				line_lexer(front, back, line, i + len);
				return ;
			}
		}
		i++;
	}
	if (i > pos)
	{
		len = i - pos;
		lexer_queue_push(front, back, ft_strndup(line + pos, len), WORD);
		line_lexer(front, back, line, i);
		return ;
	}
}

void	lexer(void)
{
	t_lexeme	*front;
	t_lexeme	*back;

	front = NULL;
	back = NULL;
	if (!g_term.line_in)
		return ;
	line_lexer(&front, &back, g_term.line_in, 0);
	
	t_lexeme *tmp = front;
	while (tmp)
	{
		ft_printf_fd(STDERR_FILENO, "TYPE: %s, STR: %s\n", g_term.symbls[tmp->set], tmp->data);
		tmp = tmp->next;
	}

	/*
	while (test)
	{
		t_lexeme *tmp = test;
		test = test->next;
		free (tmp);
	}
	*/
	plant_tree(front);
	free(g_term.line_in);
	g_term.line_in = NULL;
	ft_printf_fd(STDERR_FILENO, "i tried\n");
}
