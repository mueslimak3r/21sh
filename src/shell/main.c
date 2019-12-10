/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/28 22:21:55 by alkozma           #+#    #+#             */
/*   Updated: 2019/12/09 19:07:44 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

void		shell_loop(void)
{
	int			quit;
	t_stats		stats;
	t_node		*tree;
	int			res;

	quit = 0;
	res = 0;
	tree = NULL;
	read_rcfile();
	while (!quit)
	{
		shell_reset_stuff(&stats);
		if ((res = get_input()) < 0)
			break ;
		if (res == 0)
			continue ;
		tree = lexer(g_term.curr_buff->buff_str);
		recurse(tree, &stats);
		if (g_term.pid > -1)
			waitpid(g_term.pid, 0, 0);
		empty_buffer(stats.f_d);
		clean_tree(tree);
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
}

int			main(void)
{
	if (!(validate_term()))
		return (0);
	init_env();
	init_term();
	set_sighandle();
	define_symbols();
	ft_bzero(g_alias, sizeof(t_ht*) * HT_OVERHEAD);
	print_banner(STDERR_FILENO);
	g_term.buff = NULL;
	g_term.curr_buff = NULL;
	shell_loop();
	reset_term();
	return (0);
}
