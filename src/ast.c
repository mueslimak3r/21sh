#include "ftshell.h"

int		make_tree(t_ast **root, char *line, int i)
{
    t_ast	*new;
	t_token	*head;
	int		ret;
	int		op;

	head = NULL;
	ret = i;
    if (!*root)
    {
        *root = ft_memalloc(sizeof(t_ast));
        new = *root;
		return(make_tree((*root)->));
    }
    else
    {
        new = ft_memalloc(sizeof(t_ast));
    }
	ret += get_token(line, ret, &head);
	ret += get_token(line, ret, &head);
    return (i);
}