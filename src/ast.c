#include "ftshell.h"
/*
int		make_tree(t_ast **root, char *line, int i)
{
    t_ast	*new;
	t_token	*head;
	int		ret;

	head = NULL;
	ret = i;
    if (!*root)
        ret += get_token(line, ret, &head);
    ret += get_token(line, ret, &head);
    if (!*root)
    {
        *root = ft_memalloc(sizeof(t_ast));
        new = *root;
    }
    else
        new = ft_memalloc(sizeof(t_ast));
    return (ret);
}
*/

void    print_tokens(t_token *tokens)
{
    while (tokens)
	{
		ft_printf("[%s] (%d)\n", tokens->name, tokens->type);
		tokens = tokens->next;
	}
}

void    parse_tokens(t_token *tokens)
{
    print_tokens(tokens);
}