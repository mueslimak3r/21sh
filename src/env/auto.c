/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   auto.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/25 10:34:13 by alkozma           #+#    #+#             */
/*   Updated: 2019/11/13 22:15:56 by alkozma          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

static int	last_slash(const char *in)
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

char		*match_sub(t_st_node **tree, char *str)
{
	t_st_node	*tmp;
	int			i;

	i = 0;
	tmp = *tree;
	if (!str)
		return (tmp->str);
	while (tmp->children[(int)str[i]])
		tmp = tmp->children[(int)str[i++]];
	return (ft_strjoin(str + i, tmp->str + 1));
}

void		add_to_tree(t_st_node **tree, char *str)
{
	t_st_node	*tmp;

	tmp = *tree;
	if (!tmp)
	{
		tmp = malloc(sizeof(t_st_node));
		tmp->parent = NULL;
		tmp->str = ft_strdup(str);
	}
	while (tmp->children[(int)*str])
		tmp = tmp->children[(int)*str++];
	while (*str)
	{
		tmp->children[(int)*str] = malloc(sizeof(t_st_node));
		tmp->children[(int)*str]->str = ft_strdup(str);
		tmp->children[(int)*str]->parent = tmp;
		tmp = tmp->children[(int)*str];
		str++;
	}
	while (tmp->parent)
		tmp = tmp->parent;
	*tree = tmp;
}

void		get_dir(t_st_node **tree, char *path)
{
	struct stat		filestat;
	DIR				*curdir;
	struct dirent	*curfile;

	if (lstat(path, &filestat) < 0 || !(curdir = opendir(path)))
	{
		ft_printf_fd(STDERR_FILENO, "help\n");
		return ;
	}
	while ((curfile = readdir(curdir)) != NULL)
		add_to_tree(tree, curfile->d_name);
}

void		auto_complete(void)
{
	char	*in;
	int		ls;
	char	*dir;
	char	*str;
	char	*xstr;
	char	**split;
	int		i;
	char	*treestr;
	t_st_node	*tree;

	/*if (!g_term.curr_buff->rope)
		return ;
	treestr = rope_getline(g_term.curr_buff->rope, 1);*/
	tree = NULL;
	split = ft_strsplit(treestr, ' ');
	i = 0;
	str = NULL;
	in = NULL;
	while (split[i])
	{
		if (in)
			free(in);
		in = ft_strdup(split[i]);
		free(split[i]);
		split[i] = NULL;
		i++;
	}
	ls = last_slash(in);
	if (ls < 0)
		return ;
	dir = ft_strsub(in, 0, ls);
	str = ft_strsub(in, ls + 1, ft_strlen(in));
	xstr = path_expansions(dir);
	free(dir);
	get_dir(&tree, xstr);
	//add_to_rope(match_sub(&tree, str));
	reprint_buffer(g_term.curr_buff);
	dir = NULL;
}
