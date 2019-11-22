#include "ftshell.h"

/*
** abstract
** Creates a new empty EXPR node and makes itself the parent of the
** 		passed node.
** Replaces passed node's position in parent's list of children.
*/

t_node			*abstract(t_node *node)
{
	t_node	*tmp;
	t_node	*new;

	new = new_abstract_node(node);
	if (!node)
		return (new);
	if (!node->parent)
	{
		node->parent = new;
		new->parent = NULL;
	}
	if ((tmp = node->parent->children) && tmp != node)
	{
		while (tmp->next && tmp->next != node)
			tmp = tmp->next;
		tmp->next = new;
	}
	node->next = NULL;
	return (new);
}

t_node			*invertify(t_node *head)
{
	t_node	*tmp;
	t_node	*lst;
	t_node	*nxt;

	if (!head || !head->children)
		return (NULL);
	tmp = head->children;
	nxt = NULL;
	lst = NULL;
	while (tmp)
	{
		nxt = tmp->next;
		tmp->next = lst;
		lst = tmp;
		tmp = nxt;
	}
	head->children = lst;
	return (head->parent ? head->parent : abstract(head));
}

/*
** clean_tree
** Recursively frees the tree and associated lexemes.
*/

void			clean_tree(t_node *head)
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

int				count_pipes(t_node *node)
{
	int		ret;

	ret = 0;
	while (node)
	{
		if (node->lexeme && (node->lexeme->set == PIPE
			|| node->lexeme->set == LESS
			|| node->lexeme->set == RDGREAT
			|| node->lexeme->set == GREAT
			|| node->lexeme->set == RDLESS))
		{
			ret++;
		}
		node = node->next;
	}
	return (ret);
}