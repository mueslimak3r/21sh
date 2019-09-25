#include "ftshell.h"

void    exec_node(t_ast *node, t_stats *ret, t_stats *l_stats)
{
    t_stats r_stats;

    r_stats.exit = 0;
    run_dispatch(node->token->args, &g_term.env);
    dispatch_tree(&node->right, &r_stats);
    if (l_stats->exit || r_stats.exit)
        ret->exit = 1;
}

int     dispatch_tree(t_ast **tree, t_stats *ret)
{
    t_stats l_stats;

    l_stats.exit = 0;
    if (!*tree)
        return (0);
    if (ft_strcmp((*tree)->token->name, "exit") == 0)
    {
        free(*tree);
        *tree = NULL;
        ret->exit = 1;
        return (0);
    }
    dispatch_tree(&(*tree)->left, &l_stats);
    //printf("at node: -%s-\n", (*tree)->token->name);
    exec_node(*tree, ret, &l_stats);
    free(*tree);
    *tree = NULL;
    return (1);
}

int     parse_tree(t_ast **tree)
{
    t_stats stats;

    stats.exit = 0;
    //print_tree(*tree);
    //printf("\nparsing tree!\n");
    dispatch_tree(tree, &stats);
    return (stats.exit);
}

void    print_tree(t_ast *tree)
{
    if (!tree)
        return ;
    printf("root: -%s- type: -%s-\n", tree->token->name, g_term.symbls[tree->token->set]);
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
