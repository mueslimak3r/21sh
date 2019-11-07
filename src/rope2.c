/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rope2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alkozma <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/04 13:39:47 by alkozma           #+#    #+#             */
/*   Updated: 2019/11/06 17:56:55 by alkozma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"
#include <stdio.h>

typedef struct s_rope_node	t_rope_node;
typedef struct s_orphan		t_orphan;

struct s_rope_node
{
	int			length;
	t_rope_node	*parent;
	t_rope_node	*left;
	t_rope_node	*right;
	char		*str;
};

struct s_orphan
{
	t_rope_node	*node;
	t_orphan	*next;
};

t_rope_node		*rope_idx(t_rope_node *head, int pos)
{
	t_rope_node	*tmp;

	tmp = head;
	if (!tmp)
		return (NULL);
	if (tmp->left && tmp->length < pos)
		return (rope_idx(tmp->left, pos - tmp->length));
	else if (tmp->right && tmp->length >= pos)
		return (rope_idx(tmp->right, pos));
	return (tmp);
}

int				sum_length(t_rope_node *head)
{
	if (!head)
		return (0);
	return ((head->left || head->right ? 0 : head->length) +
			sum_length(head->left) + sum_length(head->right));
}

t_rope_node		*rope_concat(t_rope_node *l, t_rope_node *r)
{
	t_rope_node	*new;

	ft_printf_fd(STDERR_FILENO, "CONCAT\n");
	new = malloc(sizeof(t_rope_node));
	new->left = l;
	new->right = r;
	new->length = l ? sum_length(l) : 0;
	new->parent = l ? l->parent : NULL;
	ft_printf_fd(STDERR_FILENO, "END CONCAT\n");
	return (new);
}

void			orphan(t_orphan **head, t_rope_node *node)
{
	t_orphan	*new;

	new = malloc(sizeof(t_orphan));
	new->node = node;
	new->node->parent = NULL;
	new->next = *head;
	*head = new;
}

t_rope_node		*collapse(t_rope_node *node)
{
	t_rope_node	*tmp;
	t_rope_node	*parent;

	tmp = node;
	if (!tmp->parent || tmp->parent->right)
		return (node);
	parent = tmp->parent;
	tmp->parent = parent->parent;
	if (parent->parent)
	{
		if (parent->parent->left == parent)
			parent->parent->left = tmp;
		else
			parent->parent->right = tmp;
	}
	free(parent);
	parent = NULL;
	return (tmp);
}

void			rebalance(t_rope_node *head)
{
	t_rope_node	*tmp;

	if (head->left)
		rebalance(head->left);
	if (head->right)
		rebalance(head->right);
	if (head->left && head->right && head->left->length < head->right->length)
	{
		tmp = head->left;
		head->left = head->right;
		head->right = tmp;
	}
	head->length = head->left ? sum_length(head->left) : head->length;
}

t_rope_node		*rebuild_orphans(t_orphan *head)
{
	t_orphan	*tmp;
	t_rope_node	*ret;
	t_orphan	*buf;

	tmp = head;
	ret = NULL;
	while (tmp)
	{
		if (!ret)
			ret = tmp->node;
		else
		{
			if (ret->length < tmp->node->length)
				ret = rope_concat(tmp->node, ret);
			else
				ret = rope_concat(ret, tmp->node);
		}
		buf = tmp;
		tmp = tmp->next;
		free(buf);
	}
	return (ret);
}

t_rope_node		*rope_split(t_rope_node **headref, int pos)
{
	t_rope_node	*head;
	t_rope_node	*leaf;
	t_rope_node	*tmp;
	t_rope_node	*ret;
	t_orphan	*orphans;

	head = *headref;
	leaf = rope_idx(head, pos);
	ret = NULL;
	tmp = NULL;
	while (leaf != head)
	{
		tmp = leaf;
		leaf = leaf->parent;
		if (leaf->right)
		{
			if (leaf->parent)
				leaf->parent->length -= leaf->right->length;
			orphan(&orphans, leaf->right);
			leaf = collapse(leaf->left);
		}
	}
	return (rebuild_orphans(orphans));
}

void			rope_print(t_rope_node *rope)
{
	if (!rope)
		return ;
	ft_printf_fd(STDERR_FILENO, "printing\n");
	if (rope->left)
		rope_print(rope->left);
	else if (!rope->left)
	{
		printf("[%s]\n", rope->str);
		return ;
	}
	if (rope->right)
		rope_print(rope->right);
}

t_rope_node		*rope_insert(t_rope_node *rope, char *data, int pos)
{
	t_rope_node	*tmp;
	t_rope_node	*tmp2;
	t_rope_node	*new;

	new = malloc(sizeof(t_rope_node));
	new->str = ft_strdup(data);
	new->length = ft_strlen(data);
	new->left = NULL;
	new->right = NULL;
	new->parent = NULL;
	if (!rope)
		return (new);
	tmp = rope_split(&rope, pos);
	tmp2 = rope_concat(new, tmp);
	return (rope_concat(rope->right, tmp2));
}

void			rope_diagnostic(void)
{
	char	test_input[] = "Testing!";
	char	test_concat[] = "concat ";
	char	test_concat2[] = "third";
	t_rope_node		*test;

	test = NULL;
	test = rope_insert(test, test_input, 0);
	test = rope_insert(test, test_concat, 1);
	test = rope_insert(test, test_concat2, 5);
	rope_print(test);
	return ;
}
