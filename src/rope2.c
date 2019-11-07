/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rope2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/04 13:39:47 by alkozma           #+#    #+#             */
/*   Updated: 2019/11/07 00:23:41 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"
#include <stdio.h>

typedef struct s_rope_node	t_rope_node;
typedef struct s_orphan		t_orphan;

struct s_rope_node
{
	int			length;
	int			removed_length;
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

t_rope_node		*rope_idx(t_rope_node *head, int *pos)
{
	t_rope_node	*tmp;

	tmp = head;
	if (!tmp || *pos < 1)
		return (NULL);
	if (tmp->right && *pos > tmp->length)
	{
		*pos = *pos - (*pos == tmp->length ? 0 : tmp->length);
		return (rope_idx(tmp->right, pos));
	}
	else if (tmp->left && *pos <= tmp->length)
		return (rope_idx(tmp->left, pos));
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

	new = NULL;
	ft_printf_fd(STDERR_FILENO, "CONCAT\n");
	if (l && r)
	{
		new = malloc(sizeof(t_rope_node));
		new->left = l;
		new->right = r;
		new->removed_length = 0;
		new->str = NULL;
		new->length = l ? sum_length(l) : 0;
		new->parent = l ? l->parent : NULL;
	}
	if (l)
		l->parent = (l && r) ? new : NULL;
	if (r)
		r->parent = (l && r) ? new : NULL;
	if (new)
		return (new);
	ft_printf_fd(STDERR_FILENO, "END CONCAT\n");
	return (l ? l : r);
}
/*
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
	static int n;

	tmp = head;
	ret = NULL;
	while (tmp)
	{
		ft_printf_fd(STDERR_FILENO, "%d yo\n", ++n);
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
*/
t_rope_node		*rope_prune_singles(t_rope_node *start)
{
	t_rope_node *curr = start;
	t_rope_node *last = NULL;
	while (curr)
	{
		curr->length -= curr->removed_length;
		if (last && last->removed_length)
		{
			curr->length -= last->removed_length;
			curr->removed_length += last->removed_length;
			last->removed_length = 0;
		}
		t_rope_node *leaf = NULL;
		if (!curr->str && (!curr->left || !curr->right))
		{
			t_rope_node *parent = curr->parent;
			leaf = curr->left ? curr->left : curr->right;
			if (leaf)
			{
				leaf->parent = curr->parent;
				if (leaf->parent && leaf->parent->left == curr)
					leaf->parent->left = leaf;
				else if (leaf->parent && leaf->parent->right == curr)
					leaf->parent->right = leaf;
				leaf->removed_length = curr->removed_length;
			}
			else
				parent = curr->parent;
			free(curr);
			last = leaf;
			curr = parent;
		}
		else
		{
			last = curr;
			curr = curr->parent;
		}
	}
	if (last && last->removed_length)
		last->removed_length = 0;
	return (last);
}

t_rope_node		*split_node(t_rope_node *start, int pos)
{
	t_rope_node	*new;

	ft_printf_fd(STDERR_FILENO, "split pos: %d s: %s\n", pos, start->str);
	new = malloc(sizeof(t_rope_node));
	if (!new)
		return (NULL);
	new->left = NULL;
	new->right = NULL;
	new->removed_length = 0;
	pos = pos == 0 ? 0 : pos - 1;
	new->length = ft_strlen(start->str) - pos;
	new->str = ft_strdup(start->str + pos);
	//ft_printf_fd(STDERR_FILENO, "split pos: %d s: %s\n", pos, start->str);
	ft_memset(start->str + pos, 0, new->length);
	new->parent = NULL;
	start->removed_length = new->length;
	return (new);
}

t_rope_node		*rope_split(t_rope_node **headref, int pos)
{
	t_rope_node	*head;
	t_rope_node	*leaf;
	t_rope_node	*tmp;
	t_rope_node	*ret;
	t_orphan	*orphans;
	static int n;

	head = *headref;
	leaf = rope_idx(head, &pos);
	t_rope_node *start;
    t_rope_node *rtree;
	if (pos > 1)
	{
        rtree = split_node(leaf, pos);
        start = leaf;
    }
    else
    {
		rtree = leaf;
		start = (leaf->parent) ? leaf->parent : NULL;
		start->removed_length = sum_length(leaf);
		if (leaf->parent)
		{
			if (leaf->parent->left == leaf)
				leaf->parent->left = NULL;
			else
				leaf->parent->right = NULL;
		}
		rtree->parent = NULL;
    }
	//if (leaf->parent)
	//	leaf->parent->length -= sum_length(leaf) - (pos == 0 ? 0 : pos - 1);
	if (!start)
	{
		*headref = NULL;
		ft_printf_fd(STDERR_FILENO, "start is null\n");
		return (rtree);
	}
	t_rope_node *curr = start;
	t_rope_node *last = NULL;
	ft_printf_fd(STDERR_FILENO, "orphaning\n");
	while (curr)
	{
		if (last && last == curr->left)
		{
			curr->removed_length += sum_length(curr->right);
			rtree = rope_concat(rtree, curr->right);
			curr->right = NULL;
		}
		last = curr;
		curr = curr->parent;
	}
	ft_printf_fd(STDERR_FILENO, "pruning\n");
	*headref = rope_prune_singles(start);
	return (rtree);
}

void			debug_print(t_rope_node *rope, int depth)
{
	if (!rope)
		return ;
	//ft_printf_fd(STDERR_FILENO, "printing\n");
	if (!rope->str)
		ft_printf_fd(STDERR_FILENO, "NODE[\nd: %d size: %d left:|", depth, rope->length);
	if (rope->left)
		debug_print(rope->left, depth + 1);
	else if (rope->str)
	{
		ft_printf_fd(STDERR_FILENO, "DATA{d: %d s: %s size: %d}", depth, rope->str, rope->length);
		return ;
	}
	ft_printf_fd(STDERR_FILENO, "|right:|");
	if (rope->right)
		debug_print(rope->right, depth + 1);
	ft_printf_fd(STDERR_FILENO, "|\n");
}

void			rope_print(t_rope_node *rope)
{
	if (!rope)
		return ;
	//ft_printf_fd(STDERR_FILENO, "printing\n");
	if (rope->left)
		rope_print(rope->left);
	else if (rope->str)
	{
		ft_printf_fd(STDERR_FILENO, "[%s]", rope->str);
		return ;
	}
	if (rope->right)
		rope_print(rope->right);
}

t_rope_node		*rope_insert(t_rope_node *rope, char *data, int pos)
{
	t_rope_node	*tmp = NULL;
	t_rope_node	*tmp2 = NULL;
	t_rope_node	*new = NULL;

	new = malloc(sizeof(*new));
	new->str = ft_strdup(data);
	new->length = ft_strlen(data);
	new->removed_length = 0;
	new->left = NULL;
	new->right = NULL;
	new->parent = NULL;
	if (!rope)
		return (new);
	if (pos <= sum_length(rope))
	{
		ft_printf_fd(STDERR_FILENO, "before:\n");
		rope_print(rope);
		ft_printf_fd(STDERR_FILENO, "\n");

		tmp = rope_split(&rope, pos);
		ft_printf_fd(STDERR_FILENO, "after left:\n");
		rope_print(rope);
		ft_printf_fd(STDERR_FILENO, "\n");

		ft_printf_fd(STDERR_FILENO, "after right:\n");
		rope_print(tmp);
		ft_printf_fd(STDERR_FILENO, "\n");
		tmp2 = rope_concat(new, tmp);
	}
	else
		tmp2 = new;
	return (rope_concat(rope, tmp2));
}

void			rope_diagnostic(void)
{
	char	test_input[] = "second**third**";
	char	test_concat[] = "concat ";
	char	test_concat2[] = "heyyy";
	t_rope_node		*test;

	test = NULL;
	test = rope_insert(test, test_input, 1);
	test = rope_insert(test, test_concat, 8);
	
	ft_printf_fd(STDERR_FILENO, "after first join:\n");
	debug_print(test, 0);
	ft_printf_fd(STDERR_FILENO, "\n");

	test = rope_insert(test, test_concat2, 50);

	ft_printf_fd(STDERR_FILENO, "\n\nresult:\n");
	rope_print(test);
	ft_printf_fd(STDERR_FILENO, "\n");
	return ;
}
