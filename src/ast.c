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

void    print_tree(t_ast *tree)
{
    if (!tree)
        return ;
    printf("root: %s ", tree->token->name);
    if (tree->left)
        printf("left: %s ", tree->left->token->name);
    if (tree->right)
        printf("right: %s ", tree->right->token->name);
    printf("\n");
    print_tree(tree->left);
    print_tree(tree->right);
}