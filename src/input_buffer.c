/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_buffer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/25 03:27:26 by calamber          #+#    #+#             */
/*   Updated: 2019/10/30 05:38:02 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

const int LEAF_LEN = 2; 

// Function that creates a Rope structure. 
// node --> Reference to pointer of current root node 
//   l  --> Left index of current substring (initially 0) 
//   r  --> Right index of current substring (initially n-1) 
//   par --> Parent of current node (Initially NULL) 
void createRopeStructure(t_rope **node, t_rope *par, 
                         char *a, int l, int r) 
{
	t_rope *tmp;
    if (!(tmp = ft_memalloc(sizeof(t_rope))))
		return ;
    tmp->left = NULL;
	tmp->right = NULL; 

    // We put half nodes in left subtree 
    tmp->parent = par; 

    // If string length is more 
    if ((r - l) > LEAF_LEN) 
    { 
        tmp->str = NULL; 
        tmp->lCount = (r - l) / 2; 
        *node = tmp;
        int m = (l + r) / 2;
        createRopeStructure(&(*node)->left, *node, a, l, m); 
        createRopeStructure(&(*node)->right, *node, a, m + 1, r); 
    } 
    else
    {
        *node = tmp; 
        tmp->lCount = (r - l); 
        int j = 0;
        tmp->str = ft_memalloc(sizeof(char) * LEAF_LEN + 1);
		if (!tmp->str)
		{
			if (tmp)
				free(tmp);
			*node = NULL;
			return ;
		}
        for (int i = l; i <= r; i++)
            tmp->str[j++] = a[i];
		ft_printf_fd(STDERR_FILENO, "size: %d, str: %s\n", tmp->lCount, tmp->str);
    } 
}

// Function that prints the string (leaf nodes) 
void printstring(t_rope *r) 
{ 
    if (r==NULL) 
        return; 
    if (r->left==NULL && r->right==NULL) 
        ft_putstr_fd(r->str, STDERR_FILENO); 
    printstring(r->left); 
    printstring(r->right); 
}

void free_rope(t_rope **r) 
{ 
    if (*r==NULL) 
        return;
    free_rope(&(*r)->left); 
    free_rope(&(*r)->right);
	free(*r);
	*r = NULL;
}

int	count_rope(t_rope *r) 
{
    if (r==NULL) 
        return (0);
	return (count_rope(r->right) + r->lCount);
}

void	join_rope_helper(t_rope *r, char *ret, int size, int *pos)
{
	if (r==NULL)
        return;
    if (r->left==NULL && r->right==NULL)
	{
		for (int i = 0; i < ft_strlen(r->str) && *pos < size; i++)
		{
			ret[*pos] = r->str[i];
			(*pos)++;
		}
	}
	join_rope_helper(r->left, ret, size, pos);
    join_rope_helper(r->right, ret, size, pos);
}

char	*join_rope(t_rope *r)
{
	char *ret;
	int	pos;
	int	count;

	if (!r)
		return (NULL);
	pos = 0;
	count = count_rope(r);
	ft_printf_fd(STDERR_FILENO, "count: %d\n", count);
	if (!(ret = ft_memalloc(sizeof(char) * count + 1)))
		return (NULL);
	join_rope_helper(r, ret, count, &pos);
	return (ret);
}

/*
// Function that efficiently concatenates two strings 
// with roots root1 and root2 respectively. n1 is size of 
// string represented by root1. 
// root3 is going to store root of concatenated Rope. 
void concatenate_rope(t_rope *root3, t_rope *root1, t_rope *root2, int n1) 
{
    // Create a new Rope node, and make root1  
    // and root2 as children of tmp. 
    t_rope *tmp = ft_memalloc(sizeof(t_rope)); 
    tmp->parent = NULL; 
    tmp->left = root1; 
    tmp->right = root2; 
    root1->parent = tmp;
	root2->parent = tmp; 
    tmp->lCount = n1; 
  
    // Make string of tmp empty and update  
    // reference r
    tmp->str = NULL; 
    root3 = tmp;
} 
*/

void		tbuff_push(t_tbuff **buff, char *s)
{
	t_tbuff *new;

	if (!s || !*s)
		return ;
	while (*s == 27)
		s++;
	if (!*s)
		return ;
	if (!(new = ft_memalloc(sizeof(t_tbuff))))
		return ;
	if (*buff)
	{
		new->prev = (*buff)->prev;
		if ((*buff)->prev)
			(*buff)->prev->next = new;
		(*buff)->prev = new;
		new->next = *buff;
	}
	else
	{
		new->next = NULL;
		new->prev = NULL;
	}
	new->rope = NULL;
	ft_printf_fd(STDERR_FILENO, "len of buff input: %d\n", (int)ft_strlen(s));
	createRopeStructure(&new->rope, NULL, s, 0, ft_strlen(s) - 1);
	new->size = ft_strlen(s);
	*buff = new;
}

/*
char		*tbuff_peek(t_tbuff *buff)
{
	if (!buff || !(buff->rope))
		return (NULL);
	ft_printf_fd(STDERR_FILENO, "peeking\n");
	return (join_rope(buff->rope));
}
*/
void		tbuff_free(t_tbuff **buff)
{
	t_tbuff *tmp;

	if (!buff || !*buff)
		return ;
	while (*buff)
	{
		tmp = *buff;
		*buff = (*buff)->next;
		if (tmp == *buff)
			*buff = NULL;
		if (tmp->rope)
			free_rope(&tmp->rope);
		free(tmp);
	}
	*buff = NULL;
}

void		tbuff_print(t_tbuff *buff)
{
	char	*s;

	ft_printf_fd(STDERR_FILENO, "start of buff\n");
	while (buff)
	{
		s = join_rope(buff->rope);
		if (s)
		{
			ft_printf_fd(STDERR_FILENO, "%s\n", s);
			ft_strdel(&s);
		}
		else
			ft_printf_fd(STDERR_FILENO, "buff string null\n");
		buff = buff->next;
	}
	ft_printf_fd(STDERR_FILENO, "end of buff\n");
}