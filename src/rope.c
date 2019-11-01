//#include "ftshell.h"
# include "../libft/libft.h"

typedef struct s_rope       t_rope;
typedef struct s_rope_node  t_rope_node;

struct s_rope_node
{
    char        *value;
    t_rope_node *parent;
    t_rope_node *left;
    t_rope_node *right;
};

struct s_rope
{
    t_rope_node *root;
    int         size;
};

t_rope  *rope_new(void)
{
    t_rope *new;

    new = ft_memalloc(sizeof(t_rope));
    if (!new)
        return (NULL);
    new->root = NULL;
    new->size = 0;
    return (new);
}

void    rope_del_nodes(t_rope_node *node)
{
    if (!node)
        return ;
    rope_del_nodes(node->left);
    rope_del_nodes(node->right);
    free(node);
}

t_rope  *rope_delete(t_rope *rope)
{
    if (!rope)
        return (NULL);
    rope_del_nodes(rope->root);
    return (NULL);
}

void    rope_insert(t_rope *rope, int i, char *s)
{

}

void    rope_index(t_rope *rope, int i)
{

}

t_rope  *rope_concat(t_rope *rope1, t_rope *rope2)
{

}

void    rope_split(t_rope *rope, t_rope **lhalf, t_rope **rhalf, int i)
{

}