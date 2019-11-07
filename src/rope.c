//#include "ftshell.h"
# include "../libft/libft.h"

typedef struct s_rope       t_rope;
typedef struct s_rope_node  t_rope_node;

struct s_rope_node
{
    int         lsize;
    char        *value;
    t_rope_node *parent;
    t_rope_node *left;
    t_rope_node *right;
};

struct s_rope
{
    t_rope_node *root;
    size_t      node_count;
    size_t      char_count;
};

t_rope  *rope_new(void)
{
    t_rope *new;

    new = ft_memalloc(sizeof(t_rope));
    if (!new)
        return (NULL);
    new->root = NULL;
    new->node_count = 0;
    new->char_count = 0;
    return (new);
}

t_rope  *rope_delete(t_rope *rope)
{
    if (!rope)
        return (NULL);
    if (!rope->root)
    {
        free(rope);
        return (NULL);
    }
    t_rope_node *tmp = rope->root;
    while (tmp)
    {
        t_rope_node *child = tmp;
        if (child->left)
        {
            while (child->left)
                child = child->left;
            if (child->parent)
                child->parent->left = NULL;
            tmp = child->parent;
            free (child);
        }
        else if (child->right)
        {
            child = child->right;
            if (child->left)
            {
                tmp = child;
                continue ;
            }
            if (child->right)
            {
                tmp = child->right;
                tmp->parent = child->parent;
                if (child->parent)
                    child->parent->right = tmp;
            }
            tmp = child->parent;
            free (child);
        }
        else
        {
            if (child->parent)
            {
                tmp = child->parent;
                if (tmp)
                {
                    if (tmp->left == child)
                        tmp->left = NULL;
                    else
                        tmp->right = NULL;
                }
            }
            free(child);
        }
    }
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
    if (!rope || !rope->root)
        return ;
    t_rope_node *tmp = rope->root;
    while (tmp && !(tmp->value))
    {
        while (tmp->lsize < i)
        {
            i -= tmp->lsize;
            tmp = tmp->right;
        }
        while (tmp->lsize > i)
            tmp = tmp->left;
        while (tmp->lsize == i && tmp->left)
            tmp = tmp->left;
    }
    char *lvalue = NULL;
    char *rvalue = NULL;
    int left_lsize = tmp->lsize;
    int right_lsize;
    if (!tmp->value)
        return (NULL);
    t_rope_node *start;
    if (i > 1)
        new = split_node(tmp, i);
}
