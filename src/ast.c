#include "ftshell.h"

int     parse_tree(t_ast **tree)
{
    print_tree(*tree);
    return (0);
}

void    print_tree(t_ast *tree)
{
    if (!tree)
        return ;
    printf("root: -%s-\n", tree->token->name);
    printf("root args:\n");
    for (int i = 0; (tree->token->args)[i]; i++)
        printf("%s\n", (tree->token->args)[i]);
    if (tree->parent)
        printf("parent: -%s- ", tree->parent->token->name);
    if (tree->left)
        printf("left: -%s- ", tree->left->token->name);
    if (tree->right)
        printf("right: -%s- ", tree->right->token->name);
    printf("\nend\n");
    print_tree(tree->left);
    print_tree(tree->right);
}
