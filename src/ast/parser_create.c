#include "ftshell.h"

t_node              *new_abstract_node(t_node *node)
{
	t_node	*new;

	new = malloc(sizeof(*new));
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
	return (new);
}

/*
** new_node
** Creates a new node with a passed type, lexeme and parent.
** Will add to parent's list of children.
*/

t_node			*new_node(enum e_nodetype set, t_lexeme *lexeme,
		t_node *parent, int dir)
{
	t_node	*new;
	t_node	*tmp;

	new = ft_memalloc(sizeof(*new));
	if (!new)
		return (NULL);
	new->set = set;
	new->lexeme = lexeme;
	new->parent = parent;
	new->next = dir ? parent->children : NULL;
	new->evaluated = 0;
	new->children = NULL;
	if (!parent)
		return (new);
	tmp = parent->children;
	if (dir)
		return ((parent->children = new));
	while (tmp && tmp->next)
		tmp = tmp->next;
	if (tmp)
		tmp->next = new;
	else
		parent->children = new;
	return (new);
}

/*
** concat_node
** Given a node, returns a string array of the data of the children's lexemes.
*/

static int			concat_get_size(t_node *node, t_redir **list)
{
	t_node	*tmp;
	int		ret;

	ret = 0;
	tmp = node->children;
	while (tmp)
	{
		if (tmp->set == EXEC || (tmp->set >= FD_R && tmp->set <= FD_A)
				|| tmp->set == ARG)
			ret += tmp->lexeme ? 1 : 0;
		else if (tmp->set == EXPR)
			redir_pipes(tmp, list);
		tmp = tmp->next;
	}
	return (ret);
}

char				**concat_node(t_node *node, t_redir **list)
{
	char	**ret;
	t_node	*tmp;
	int		sz;
	int		i;

	if (!node)
		return (NULL);
	tmp = node->children;
	ret = NULL;
	sz = concat_get_size(node, list);
	tmp = node->children;
	ret = malloc(sizeof(char*) * (sz + 1));
	i = 0;
	while (tmp)
	{
		if (tmp->set == ARG || tmp->set == EXEC)
			if (tmp->lexeme && tmp->lexeme->data)
				ret[i++] = ft_strdup(tmp->lexeme->data);
		tmp = tmp->next;
	}
	ret[i] = 0;
	i = 0;
	return (ret);
}