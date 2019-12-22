/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   auto.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/25 10:34:13 by alkozma           #+#    #+#             */
/*   Updated: 2019/11/21 16:12:35 by alkozma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

int	last_slash(const char *in)
{
	int		ret;
	int		i;

	ret = -1;
	i = 0;
	while (*in)
	{
		if (*in == '/')
			ret = i;
		in++;
		i++;
	}
	return (ret);
}

struct s_st	*st_new_node(char *str)
{
	struct s_st	*new;

	new = malloc(sizeof(*new));
	new->access[0] = 0;
	new->access[1] = 0;
	new->str = str;
	return (new);
}

void	st_insert_str(char *str, struct s_st **tree)
{
	struct s_st	*head;

	ft_printf_fd(STDERR_FILENO, "adding %s to tree\n", str);
	head = *tree;
	if (!head)
	{
		*tree = st_new_node(NULL);
		head = *tree;
	}
	while (head && (uint64_t)(head->access[*str < 64 ? 0 : 1] >> (*str % 64)) & 1)
		head = head->children[(int)(*str++)];
	while (*str)
	{
		head->access[*str < 64 ? 0 : 1] += ((uint64_t)1 << (*str % 64));
		head->children[(int)(*str)] = st_new_node(str);
		head = head->children[(int)(*str)];
		str++;
	}
}

char	*st_match(char *sub, struct s_st *tree)
{
	struct s_st	*head;

	if (!(head = tree))
		return (NULL);
	ft_printf_fd(STDERR_FILENO, "matching %s\n", sub);
	while (head && sub && *sub && head->access[*sub < 64 ? 0 : 1] >> (*sub % 64) & 1)
		head = head->children[(int)*sub++];
	return (head ? head->str : NULL);
}

void	st_clean(struct s_st *tree)
{
	int	i;

	i = 0;
	while (i < 128 && tree)
	{
		if ((tree->access[i < 64 ? 0 : 1] >> i % 64) & 1)
			st_clean(tree->children[i]);
		i++;
	}
	free(tree);
	tree = NULL;
}
