/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/28 22:21:55 by alkozma           #+#    #+#             */
/*   Updated: 2019/11/07 21:41:12 by calamber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ftshell.h"

static bool	validate_term(void)
{
	char	*term_id;
	char	buf[2048];

	term_id = getenv("TERM");
	if (!term_id)
		ft_putstr_fd("error: $TERM variable invalid\n",
		STDERR_FILENO);
	else if (!tgetent(buf, term_id))
		ft_putstr_fd("error: tgetent failed\n",
		STDERR_FILENO);
	else
		return (true);
	return (false);
}

void		init_term(void)
{
	char	buf[150];
	char	*temp;

	temp = buf;
	tcgetattr(STDERR_FILENO, &g_term.old_term);
	tcgetattr(STDERR_FILENO, &g_term.new_term);
	g_term.new_term.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDERR_FILENO, TCSANOW, &g_term.new_term);
	if (!(tgetstr("vi", &temp)))
	{
		ft_putstr_fd("error: host terminal attr invalid\n", STDERR_FILENO);
		exit(0);
	}
	GET_SCREENSIZE;
	g_term.rows = 1;
}

void		reset_term(void)
{
	char	*temp;
	char	buf[150];

	temp = buf;
	ft_putchar_fd('\n', STDERR_FILENO);
	tcsetattr(STDERR_FILENO, TCSANOW, &g_term.old_term);
}

int			has_hd(char *thing, char *hd)
{
	char	**split;
	int		i;
	int		ret;

	if (!thing)
		return (0);
	split = ft_strsplit(thing, '\n');
	i = 0;
	ret = 0;
	while (split[i])
	{
		if (ft_strcmp(split[i], hd) == 0)
			ret = 1;
		free(split[i]);
		split[i] = NULL;
		i++;
	}
	free(split);
	split = NULL;
	return (ret);
}

int			ft_readstdin_line(int hd, char *stop)
{
	char	buf[BUFF_SIZE + 1];
	char	*tmp;
	int		ret;
	t_input	thing;

	g_term.conf.curlines = 1;
	ft_memset(buf, 0, BUFF_SIZE + 1);
	term_write(hd ? HDPROMPT : PROMPT, STDERR_FILENO, 1);
	g_term.conf.cursor[0] = ft_strlen(hd ? HDPROMPT : PROMPT);
	thing.long_form = 0;
	if (g_term.line_in)
	{
		free(g_term.line_in);
		g_term.line_in = 0;
	}
	while ((ret = read(0, &buf, 4)) >= 0)
	{
		//rope_print(g_term.curr_buff->rope);
		//if ((g_term.curr_buff->rope_buff)[0])
		//	ft_printf_fd(STDERR_FILENO, "%s", g_term.curr_buff->rope_buff);
		ft_memcpy(thing.arr_form, buf, 4);
		if ((handle_controls(thing.long_form, buf, g_term.line_in)) < 1)
		{
			// these lines will add to the rope as text is recieved.
			// we need some sort of buffer to fill before kicking back to the tree
			// to prevent new rope nodes for every character
			//
			
			tbuff_rope_add(g_term.curr_buff, g_term.curr_buff->rope_buff, buf);
			reprint_buffer(g_term.curr_buff, g_term.curr_buff->cursor);
			tmp = ft_strjoin(g_term.line_in, buf);
			if (g_term.line_in)
				free(g_term.line_in);
			g_term.line_in = tmp;
		}
		else if (thing.long_form == ENTER && !hd)
		{
			if (g_term.curr_buff && g_term.curr_buff->rope_buff[0])
			{
				g_term.curr_buff->rope = rope_insert(g_term.curr_buff->rope, g_term.curr_buff->rope_buff, g_term.curr_buff->cursor + 1);
				ft_memset(g_term.curr_buff->rope_buff, 0, LEAF_SIZE + 1);
				g_term.curr_buff->rope_buff_pos = 0;
				g_term.curr_buff->rope_buff_cursor = 0;
				reprint_buffer(g_term.curr_buff, 1);
			}
			//tbuff_push(&g_term.buff, g_term.line_in);
			return (1);
		}
		else if (thing.long_form == ENTER)
		{
			ft_printf_fd(STDERR_FILENO, "enter hd\n");
			tmp = ft_strjoin(g_term.line_in, buf);
			if (g_term.line_in)
				free(g_term.line_in);
			g_term.line_in = tmp;
			if (has_hd(tmp, stop))
				return (1);
			term_write(hd ? HDPROMPT : PROMPT, STDERR_FILENO, 1);
		}
		ft_memset(buf, 0, BUFF_SIZE + 1);
	}
	return (0);
}

void		shell_loop(void)
{
	int			quit;
	t_stats		stats;
	t_node		*tree;

	quit = 0;
	g_term.conf.termsize[0] = g_window_size.ws_col;
	g_term.conf.termsize[1] = g_window_size.ws_row;
	g_term.conf.cursor[0] = 0;
	g_term.conf.cursor[1] = 0;
	g_term.conf.curr_c = -1;
	g_term.line_in = NULL;
	g_term.buff = NULL;
	g_term.curr_buff = NULL;
	tree = NULL;
	read_rcfile();
	//rope_diagnostic();
	while (!quit)
	{

		if (!g_term.buff || (g_term.buff && g_term.buff->rope))
		{
			//ft_printf_fd(STDERR_FILENO, "making new buff\n");
			tbuff_new(&g_term.buff);
		}
		g_term.curr_buff = g_term.buff;
		if (!ft_readstdin_line(0, NULL))
			continue ;
		if (g_term.curr_buff)
		{
			ft_printf_fd(STDERR_FILENO, "BUFF:{\n");
			rope_print(g_term.curr_buff->rope);
			if ((g_term.curr_buff->rope_buff)[0])
				ft_printf_fd(STDERR_FILENO, "%s", g_term.curr_buff->rope_buff);
			ft_printf_fd(STDERR_FILENO, "\n}\n");
		}
		//ft_printf_fd(STDERR_FILENO, "done reading line: %s\n", g_term.line_in);
		stats.f_d[0] = 0;
		stats.f_d[1] = 1;
		if (!g_term.curr_buff || !g_term.curr_buff->rope)
			continue ;
		tree = lexer(rope_getline(g_term.curr_buff->rope, 1));
		//ft_printf_fd(STDERR_FILENO, "finished with lexing\n");
		free(g_term.line_in);
		g_term.line_in = NULL;
		recurse(tree, &stats);
		//ft_printf_fd(STDERR_FILENO, "finished with tree\n");
		//tbuff_print(g_term.buff);
		empty_buffer(stats.f_d);
		clean_tree(tree);
	}
	tbuff_free(&g_term.buff);
}

void		define_symbols(void)
{
	static char	*symbols[15] = {
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
	"REDIRECT",
	"IO_NUMBER",
	"NEWLINE",
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
	shell_loop();
	reset_term();
	return (0);
}
