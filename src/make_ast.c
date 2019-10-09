//FIXME: Clean up/take out this old code.
# include "ftshell.h"

int			is_operator(char *op, int pos)
{
	int		i;
	int		ret;
	

	i = 2;
	ret = 0;
	while (g_term.symbls[i])
	{
		//if (pos > 0 && ft_strncmp(op + pos - 1, g_term.symbls[i], ft_strlen(g_term.symbls[i])) == 0)
		//	return (2);
		if (ft_strncmp(op + pos, g_term.symbls[i], ft_strlen(g_term.symbls[i])) == 0)
			return (i);
		i++;
	}
	if (op && ret == 0)
		return (1);
	return (ret);
}

int			should_reparent(t_ast *node)
{
	if (!node)
		return (0);
	if (node->token->set == SEMI)
		return (1);
	return (0);
}

t_token		*make_tree_token(char *text)
{
	t_token *new;

	if (!text)
		return (NULL);
	new = ft_memalloc(sizeof(t_token));
	if (!new)
		return (NULL);
	//printf("adding token: -%s-\n", text);
	new->name = ft_get_word(text, 1);
	new->args = ft_strsplit_space(text);
	new->set =	is_operator(text, 0);
	return (new);
}

t_ast		*make_tree_node(char *input, int start, int end)
{
	t_token	*token;
	t_ast	*new;

	new = NULL;
	token = NULL;
	//printf("making node from start %d end %d\n", start, end);
	if (!input[start])
	{
		ft_printf_fd(STDERR_FILENO, "bad input\n");
		return (0);
	}
	while (ft_isspace(input[start]))
		start++;
	token = make_tree_token(ft_strsub(input, start, end - start + 1));
	if (token)
	{
		new = malloc(sizeof(t_ast));
		if (!new)
		{
			free(token);
			return (0);
		}
		new->token = token;
		new->parent = NULL;
		new->left = NULL;
		new->right = NULL;
	}
	else
		return (NULL);
	return (new);
}

int			get_tokens(t_ast **tree, t_ast *parent, t_ast **head, char *input, int curr)
{
	t_ast	*new;
	t_ast	*r_statement;

	r_statement = NULL;
	new = NULL;
	if (curr < 0)
		return (0);
	int i = curr;
	if (is_operator(input, i) == 1)
	{
		//printf("making r node\n");
		while (i >= 0 && is_operator(input, i) == 1)
			i--;
		r_statement = make_tree_node(input, ++i, curr);
		curr = --i;
	}
	if (is_operator(input, i) > 1)
	{
		//printf("making new\n");
		while (i >= 0 && is_operator(input, i) > 1)
			i--;
		new = make_tree_node(input, ++i, curr);
	}
	if (!new)
	{
		if (!r_statement)
			return (0);
		//printf("swapping\n");
		new = r_statement;
		if (!*tree)
			*tree = new;
		new->parent = parent;
	}
	else if (should_reparent(new))
	{
		//printf("reparenting\n");
		if (r_statement)
		{
			r_statement->parent = parent;
			*tree = r_statement;
		}
		new->right = *head;
		*head = new;
	}
	else
	{
		new->right = r_statement;
		new->parent = parent;
		if (!*tree)
			*tree = new;
	}
	new->left = NULL;
	get_tokens(&new->left, new, head, input, i - 1);
	return (1);
}

t_ast		*parse_input(void)
{
	t_ast	*tree;
	int		len = ft_strlen(g_term.line_in);
	tree = NULL;
	if (len < 1)
	{
		ft_strdel(&g_term.line_in);
		return (tree);
	}
	//printf("s: %s - len: %d\n", input, len);
	get_tokens(&tree, NULL, &tree, g_term.line_in, len - 1);
	ft_strdel(&g_term.line_in);
	return (tree);
}
