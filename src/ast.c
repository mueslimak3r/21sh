#include "ftshell.h"



void    make_tree(t_ast **root, char *line)
{
    t_ast *new;

    if (!*root)
    {
        *root = ft_memalloc(sizeof(t_ast));
        new = *root;
    }
    else
    {
        new = ft_memalloc(sizeof(t_ast));
    }
    while (line)
    {
        line += make_tree(&new->left, line)
        line += make_tree(&new->right, line);
    }
    return (new);
}