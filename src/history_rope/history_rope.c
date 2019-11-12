/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_rope.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/04 13:39:47 by alkozma           #+#    #+#             */
/*   Updated: 2019/11/10 22:45:41 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

t_rope_node		*rope_idx(t_rope_node *head, int *pos)
{
	t_rope_node	*tmp;

	tmp = head;
	if (!tmp || *pos < 1)
		return (NULL);
	if (tmp->right && *pos > tmp->length)
	{
		//ft_printf_fd(STDERR_FILENO, "going right pos %d len %d str: %s\n", *pos, tmp->length, tmp->str);
		*pos -= (*pos == tmp->length) ? 0 : tmp->length;
		return (rope_idx(tmp->right, pos));
	}
	else if (tmp->left && *pos <= tmp->length)
	{
		//ft_printf_fd(STDERR_FILENO, "going left pos %d len %d str: %s\n", *pos, tmp->length, tmp->str);
		return (rope_idx(tmp->left, pos));
	}
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
	//ft_printf_fd(STDERR_FILENO, "CONCAT\n");
	if (l && r)
	{
		new = malloc(sizeof(t_rope_node));
		new->left = l;
		new->right = r;
		new->removed_length = 0;
		ft_memset(new->str, 0, LEAF_SIZE + 1);
		new->length = l ? sum_length(l) : 0;
		new->parent = NULL;//l ? l->parent : NULL;
	}
	if (l)
		l->parent = (new) ? new : NULL;
	if (r)
		r->parent = (new) ? new : NULL;
	if (new)
		return (new);
	//ft_printf_fd(STDERR_FILENO, "END CONCAT\n");
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

t_rope_node		*rope_prune_singles(t_rope_node *start)
{
	t_rope_node *curr = start;
	t_rope_node *last = NULL;
	while (curr)
	{
		if (!last || (last && curr->left == last))
			curr->length -= curr->removed_length;
		if (last && last->removed_length)
		{
			//curr->length -= last->removed_length;
			curr->removed_length += last->removed_length;
			last->removed_length = 0;
		}
		t_rope_node *leaf = NULL;
		if (!curr->str[0] && (!curr->left || !curr->right))
		{
			t_rope_node *parent = curr->parent;
			leaf = curr->left ? curr->left : curr->right;
			if (leaf)
			{
				leaf->parent = curr->parent;
				leaf->removed_length = curr->removed_length;
			}
			else if (curr->parent)
				parent = curr->parent;
			if (curr->parent && curr->parent->left == curr)
				curr->parent->left = leaf;
			if (curr->parent && curr->parent->right == curr)
				curr->parent->right = leaf;
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

	//ft_printf_fd(STDERR_FILENO, "split pos: %d s: %s\n", pos, start->str);
	new = malloc(sizeof(t_rope_node));
	if (!new)
		return (NULL);
	new->left = NULL;
	new->right = NULL;
	new->removed_length = 0;
	pos = pos == 0 ? 0 : pos - 1;
	new->length = ft_strlen(start->str) - pos;
	ft_memset(new->str, 0, LEAF_SIZE + 1);
	ft_memcpy(new->str, start->str + pos, ft_strlen(start->str + pos));
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
	//t_orphan	*orphans;
	//static int n;

	head = *headref;
	int	i = pos;
	leaf = rope_idx(head, &i);
	if (!leaf)
		return (NULL);
	t_rope_node *start;
    t_rope_node *rtree;
	//ft_printf_fd(STDERR_FILENO, "splitting\n");
	if (i > 1)
	{
		//ft_printf_fd(STDERR_FILENO, "original pos: %d, pos: %d, len: %d leaf str: |%s|\n", i, pos, leaf->length, leaf->str);
        rtree = split_node(leaf, i);
        start = leaf;
    }
    else
    {
		//ft_printf_fd(STDERR_FILENO, "start split pos: %d\n", pos);
		rtree = leaf;
		start = leaf->parent;
		if (start)
		{
			start->removed_length = sum_length(leaf);
			if (start->left == leaf)
			{
				if (start->right)
				{
					//ft_printf_fd(STDERR_FILENO, "yoooo\n");
					start->removed_length += sum_length(start->right);
					rtree = rope_concat(rtree, start->right);
					start->right = NULL;
				}
				leaf->parent->left = NULL;
			}
			else
				start->right = NULL;
		}
		rtree->parent = NULL;
    }
	//ft_printf_fd(STDERR_FILENO, "finished ophaning\n");
	//if (leaf->parent)
	//	leaf->parent->length -= sum_length(leaf) - (pos == 0 ? 0 : pos - 1);
	if (!start)
	{
		*headref = NULL;
		//ft_printf_fd(STDERR_FILENO, "start is null\n");
		return (rtree);
	}
	t_rope_node *curr = start;
	t_rope_node *last = NULL;
	//ft_printf_fd(STDERR_FILENO, "orphaning\n");
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
	//ft_printf_fd(STDERR_FILENO, "pruning\n");
	*headref = rope_prune_singles(start);
	return (rtree);
}

void			debug_print(t_rope_node *rope, int depth)
{
	if (!rope)
		return ;
	//ft_printf_fd(STDERR_FILENO, "printing\n");
	if (!rope->str[0])
		ft_printf_fd(STDERR_FILENO, "NODE[d: %d size: %d left:|", depth, rope->length);
	else
	{
		ft_printf_fd(STDERR_FILENO, "DATA{d: %d s: %s size: %d}\n", depth, rope->str, rope->length);
		return ;
	}
	ft_printf_fd(STDERR_FILENO, "lsize: %d | rsize: %d",
			rope->left ? rope->left->length : 0,
			rope->right ? rope->right->length : 0);
	ft_printf_fd(STDERR_FILENO, "|\n");
	if (rope->left)
		debug_print(rope->left, depth + 1);
	if (rope->right)
		debug_print(rope->right, depth + 1);
}

void			rope_print(t_rope_node *rope)
{
	if (!rope)
		return ;
	if (rope->left)
		rope_print(rope->left);
	else if (rope->str[0])
	{
		term_write(rope->str, STDERR_FILENO, 1);
		return ;
	}
	if (rope->right)
		rope_print(rope->right);
}

int				rope_print_index(t_rope_node *rope, t_rope_node *end)
{
	if (!rope)
		return (1);
	if (rope->left && !(rope_print_index(rope->left, end)))
		return (0);
	else if (rope->str[0])
	{
		ft_printf_fd(STDERR_FILENO, "%s", rope->str);
		if (rope == end)
			return (0);
		return (1);
	}
	if (rope->right && !(rope_print_index(rope->right, end)))
		return (0);
	return (1);
}

void			rope_print_from_index(t_rope_node *rope, int i, int j)
{
	if (!rope)
		return ;
	t_rope_node *curr = NULL;
	t_rope_node	*last = NULL;
	t_rope_node	*end = NULL;
	if (i < 0 || j < 0 || j < i)
		return ;
	last = rope_idx(rope, &i);
	end = rope_idx(rope, &j);
	curr = last->parent;
	i = i == 0 ? 0 : i - 1;
	if (!last)
	{
		return ;
	}
	if (last->str[0])
	{
		//ft_printf_fd(STDERR_FILENO, "i:|%d|", i);
		ft_printf_fd(STDERR_FILENO, "%s", last->str + i);
	}
	if (last == end)
		return ;
	while (curr)
	{
		if (last && last == curr->left && !(rope_print_index(curr->right, end)))
			return ;
		last = curr;
		curr = curr->parent;
	}
	//if (last)
	//	rope_print(last->right);
}

void			rope_getline_tree(t_rope_node *rope, char *ret, int *pos, int size)
{
	if (rope->left)
		rope_getline_tree(rope->left, ret, pos, size);
	else if (rope->str[0] && *pos + rope->length <= size)
	{
		ft_memcpy(ret + *pos, rope->str, rope->length);
		*pos += rope->length;
		return ;
	}
	if (rope->right)
		rope_getline_tree(rope->right, ret, pos, size);
}

char			*rope_getline(t_rope_node *rope, int i)
{
	if (!rope)
		return (NULL);
	t_rope_node *curr = NULL;
	t_rope_node	*last = NULL;
	char		*ret = NULL;
	int			pos = 0;
	int			size;

	size = sum_length(rope);
	last = rope_idx(rope, &i);
	ret = ft_memalloc(sizeof(*ret) * (size + 1));
	if (!ret)
		return (NULL);
	curr = last->parent;
	i = i == 0 ? 0 : i - 1;
	if (last->str[0] && pos + last->length <= size)
	{
		ft_memcpy(ret + pos, last->str, last->length);
		pos += last->length;
	}
		//term_write(last->str + i, STDERR_FILENO, 1);
	while (curr)
	{
		if (last && last == curr->left)
			rope_getline_tree(curr->right, ret, &pos, size);
		last = curr;
		curr = curr->parent;
	}
	return (ret);
}

t_rope_node		*rope_insert(t_rope_node *rope, char *data, int pos)
{
	t_rope_node	*tmp = NULL;
	t_rope_node	*tmp2 = NULL;
	t_rope_node	*new = NULL;
	int			lpos = pos;
	//ft_printf_fd(STDERR_FILENO, "added rope node at %d ", pos);
	t_rope_node	*idx = rope_idx(rope, &lpos);
	lpos -= lpos == 0 ? 0 : 1;
	if (!idx && rope)
	{
		//ft_printf_fd(STDERR_FILENO, "idx and rope null\n");
		return (rope);
	}
	//ft_printf_fd(STDERR_FILENO, "s %d sr %d\n", (int)ft_strlen(data), sum_length(rope));
	//ft_printf_fd(STDERR_FILENO, "rope insert\n");
	if (idx && ft_strlen(idx->str) + ft_strlen(data) <= LEAF_SIZE)
	{
		//if (lpos == 1)
		//	ft_printf("\n");
		//ft_printf_fd(STDERR_FILENO, "idx len %d lpos %d\n", idx->length, lpos);
		ft_memmove(idx->str + lpos + ft_strlen(data), idx->str + lpos,
				ft_strlen(idx->str - lpos));
		ft_memcpy(idx->str + lpos, data, ft_strlen(data));
		//ft_printf_fd(STDERR_FILENO, "value after memmove: |%s|\n", idx->str);
		idx->removed_length = ft_strlen(data);
		idx->removed_length *= -1;
		//ft_printf_fd(STDERR_FILENO, "adding len %d at index: %d lpos: %d\n", idx->removed_length, pos, lpos);
		t_rope_node *ret = NULL;
		if (idx->parent && idx->parent->left == idx)
		{
			ret = rope_prune_singles(idx);
		}
		else if (idx->parent && idx->parent->right == idx)
		{
			//ft_printf_fd(STDERR_FILENO, "not left\n");
			//idx->length -= idx->removed_length;
			//idx->parent->parent->removed_length += idx->removed_length;
			//idx->removed_length = 0;
			if (idx->parent)
			{
				ret = rope_prune_singles(idx);

			}
		}
		else
		{
			idx->length -= idx->removed_length;
			idx->removed_length = 0;
			ret = rope;
		}
		/*
		else
		{
			ft_printf_fd(STDERR_FILENO, "else\n");
			idx->length -= idx->removed_length;
			idx->removed_length = 0;
			//ft_printf_fd(STDERR_FILENO, "else\n");
			ret = rope_prune_singles(rope);
		}
		*/
		g_term.curr_buff->rope = ret;
		move_cursor(ft_strlen(data));
		if (g_term.curr_buff)
			g_term.curr_buff->cursor += ft_strlen(data);
		return (ret);
	}
	new = malloc(sizeof(*new));
	ft_memset(new->str, 0, LEAF_SIZE + 1);
	ft_memcpy(new->str, data, ft_strlen(data));
	new->length = ft_strlen(data);
	new->removed_length = 0;
	new->left = NULL;
	new->right = NULL;
	new->parent = NULL;
	if (g_term.curr_buff)
		g_term.curr_buff->cursor += new->length;
	if (!rope)
	{
		g_term.curr_buff->rope = new;
		move_cursor(ft_strlen(data));
		return (new);
	}
	if (pos < sum_length(rope))
	{
		//ft_printf_fd(STDERR_FILENO, "before:\n");
		//rope_print(rope);
		//ft_printf_fd(STDERR_FILENO, "\n");

		tmp = rope_split(&rope, pos);
		//ft_printf_fd(STDERR_FILENO, "after left:\n");
		//rope_print(rope);
		//ft_printf_fd(STDERR_FILENO, "\n");

		//ft_printf_fd(STDERR_FILENO, "after right:\n");
		//rope_print(tmp);
		//ft_printf_fd(STDERR_FILENO, "\n");
		tmp2 = rope_concat(new, tmp);
	}
	else
		tmp2 = new;
	rope = rope_concat(rope, tmp2);
	g_term.curr_buff->rope = rope;
	move_cursor(ft_strlen(data));
	return (rope);
}


//
// this will be replaced, i just wanted to see if it
//

void	rope_free(t_rope_node *rope)
{
    t_rope_node *tmp = rope;
    while (tmp)
    {
        t_rope_node *child = tmp;
        if (child->left)
        {
            while (child->left)
                child = child->left;
            if (child->parent)
                child->parent->left = NULL;
            tmp = child->parent;
            free (child);
        }
        else if (child->right)
        {
            child = child->right;
            if (child->left)
            {
                tmp = child;
                continue ;
            }
            if (child->right)
            {
                tmp = child->right;
                tmp->parent = child->parent;
                if (child->parent)
                    child->parent->right = tmp;
            }
            tmp = child->parent;
			if (tmp)
				tmp->right = NULL;
            free (child);
        }
        else
        {
			tmp = child->parent;
            if (child->parent)
            {
                if (tmp)
                {
                    if (tmp->left == child)
                        tmp->left = NULL;
                    else
                        tmp->right = NULL;
                }
            }
            free(child);
        }
    }
}

t_rope_node		*rope_delete(t_rope_node *rope, int pos, int size)
{
	t_rope_node *middle = NULL;
	t_rope_node *right = NULL;
	t_rope_node	*ret = NULL;
	int			rsize = sum_length(rope);
	if (!rope)
		return (rope);
	if (pos <= rsize)
	{
		middle = rope_split(&rope, pos);
		if (pos + size <= rsize)
		{
			right = rope_split(&middle, size + 1);
			ret = rope_concat(rope, right);
		}
		else
			ret = rope;
		rope_free(middle);
	}
	return (ret ? ret : rope);
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
	debug_print(test, 1);
	//rope_print(test);
	ft_printf_fd(STDERR_FILENO, "\n");

	test = rope_insert(test, test_concat2, 50);

	ft_printf_fd(STDERR_FILENO, "\n\nresult1:\n");
	debug_print(test, 1);
	//rope_print(test);
	ft_printf_fd(STDERR_FILENO, "\n");

	test = rope_delete(test, 5, 5);
	ft_printf_fd(STDERR_FILENO, "\n\nresult2:\n");
	debug_print(test, 1);
	//rope_print(test);
	ft_printf_fd(STDERR_FILENO, "\n");
	return ;
}
