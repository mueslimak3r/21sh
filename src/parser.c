# include "ftshell.h"

int			is_operator(char *op, int pos)
{
	int		i;
	int		ret;
	

	i = 2;
	ret = 0;
	while (g_term.symbls[i])
	{
		if (pos > 0 && ft_strncmp(op + pos - 1, g_term.symbls[i], ft_strlen(g_term.symbls[i])) == 0)
			return (2);
		else if (ft_strncmp(op + pos, g_term.symbls[i], ft_strlen(g_term.symbls[i])) == 0)
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
		printf("bad input\n");
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

t_ast		*parse_input(char *input)
{
	t_ast	*tree;
	int		len = ft_strlen(input);
	tree = NULL;
	if (len < 1)
		return (tree);
	//printf("s: %s - len: %d\n", input, len);
	get_tokens(&tree, NULL, &tree, input, len - 1);
	return (tree);
}

/*
typedef struct s_cdata  t_cdata;

struct s_cdata
{
    char *command;
    char **args;

};

int	is_operator(char *data)
{
	if (!data || !*data)
		return (-1);
	if (*data == REDIR_L || *data == REDIR_R || *data == PIPE
			|| *data == AND || *data == WAIT)
	{
		if (*data == REDIR_L && *(data + 1) == REDIR_L)
			return (1);
		if (*data == REDIR_R && *(data + 1) == REDIR_L)
			return (1);
		else
			return (0);
	}
	return (-1);
}

enum e_tokentype	define_flags(char *data)
{
	enum e_tokentype	e;

	if (*data == AND || *data == WAIT)
		return ((e = OPERATOR));
	if (*data == REDIR_L || *data == REDIR_R || *data == PIPE)
		return ((e = REDIRECT));
	return ((e = WORD));
}

void		add_token(t_token **head, t_token *new)
{
	t_token	*old;

	old = *head;
	if (!old)
		*head = new;
	else
	{
		while (old->next)
			old = old->next;
		old->next = new;
	}
}

t_token		*new_token(char *data)
{
	t_token	*new;

	new = malloc(sizeof(t_token));
	new->type = (int)define_flags(data);
	new->name = data;
	new->next = NULL;
	return (new);
}

t_token		*parse_input(char *input)
{
	int	cur;
	int	last;
	t_token	*tokens;
	int	res;

	last = 0;
	cur = 0;
	tokens = NULL;
	while (input[cur])
	{
		if ((res = is_operator(input + cur)) >= 0)
		{
			add_token(&tokens, new_token(ft_strsub(input, last, res + cur - last == 0 ? 1 : cur - last)));
			if (last != cur && input[cur + 1])
				add_token(&tokens, new_token(ft_strsub(input, cur, 1 + res)));
			last = cur + 1 + res;
		}
		cur += 1 + (res == 1 ? res : 0);
	}
	add_token(&tokens, new_token(ft_strsub(input, last, cur - last)));
	return (tokens);
}
*/