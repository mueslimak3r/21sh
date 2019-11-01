//#include "ftshell.h"
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

}

t_rope  *rope_delete(t_rope *rope)
{

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