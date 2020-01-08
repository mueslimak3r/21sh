/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/28 22:21:55 by alkozma           #+#    #+#             */
/*   Updated: 2019/12/18 14:00:20 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

void		shell_loop(void)
{
	int			quit;
	t_stats		stats;
	//t_node		*tree;
	int			res;
	t_atom		*atoms;
	t_molecule	*molecule;

	atoms = NULL;
	molecule = NULL;
	quit = 0;
	res = 0;
	//tree = NULL;
	read_rcfile();
	while (!quit)
	{
		shell_reset_stuff(&stats);
		if ((res = get_input()) < 0)
			break ;
		if (res == 0)
			continue ;
		/*if (!(tree = lexer(g_term.buff->buff_str)))
			continue ;*/
		if (!(atoms = atomizer(g_term.buff->buff_str)))
			continue ;
		if (atoms && !(molecule = moleculizer(atoms)))
		{
			ft_printf_fd(STDERR_FILENO, "moleculizer fucked up cap'n\n");
			continue ;
		}
		reset_term();
		//recurse(tree, &stats);
		if (g_term.children)
			child_wait();
		init_term();
		empty_buffer(stats.f_d);
		//clean_tree(tree);
	}
	tbuff_free(&g_term.buff);
}

void		define_symbols(void)
{
	static char	*symbols[17] = {
	"NONE",
	"WORD",
	"EXPANSION",
	"&&",
	"<<",
	">>",
	"||",
	"|",
	";",
	"<",
	">",
	"\n",
	"<&",
	">&",
	"IO_NAME",
	">&-",
	NULL };

	g_term.symbls = symbols;
	g_term.buff = NULL;
}

void		subshell(char *input)
{
	t_stats		stats;
	t_node		*tree;

	tree = NULL;
	if (!(tree = lexer(input)))
		return ;
	reset_term();
	recurse(tree, &stats);
	if (g_term.children)
		child_wait();
	init_term();
	empty_buffer(stats.f_d);
	clean_tree(tree);
}

int			main(int ac, char **av)
{
	if (!(validate_term()))
		return (0);
	init_env();
	init_term();
	set_sighandle();
	define_symbols();
	ft_bzero(g_alias, sizeof(t_ht*) * HT_OVERHEAD);
	if (ac == 3 && !(ft_strcmp(av[1], "-c")))
		subshell(av[2]);
	else
	{
		print_banner(STDERR_FILENO);
		shell_loop();
	}
	reset_term();
	return (0);
}
