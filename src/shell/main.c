/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/28 22:21:55 by alkozma           #+#    #+#             */
/*   Updated: 2019/11/21 18:35:57 by calamber         ###   ########.fr       */
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

int			termcap_reset_cursor(int pos, int len)
{
	int		a;

	a = len - 1;
	//ft_printf_fd(STDERR_FILENO, "[%d][%d]\n", a, pos);
	while (a-- > pos)
		ft_printf_fd(STDERR_FILENO, "\b");
	return (1);
}

int			check_fd(int fd)
{
	fd_set set;
	struct timeval timeout;
	int rv;

	FD_ZERO(&set);
	FD_SET(fd, &set);
	timeout.tv_sec = 0;
	timeout.tv_usec = 10000;
	rv = select(fd + 1, &set, NULL, NULL, &timeout);
	if (rv == -1)
		return (0);
	else if (rv == 0)
		return (0);
	else
		return (1);
	return (0);
}

int			ft_readstdin_line(int hd, char *stop)
{
	char	buf[BUFF_SIZE + 1];
	char	*tmp;
	int		ret;
	t_input	thing;

	tmp = NULL;
	
	ft_memset(buf, 0, BUFF_SIZE + 1);
	ft_printf_fd(STDERR_FILENO, "\n%s", hd ? HDPROMPT : PROMPT);
	
	g_term.conf.cursor[0] = ft_strlen(hd ? HDPROMPT : PROMPT);
	g_term.conf.cursor[1] = 0;
	g_term.conf.curlines = 1;
	thing.long_form = 0;
	ret = 0;
	while (!g_term.sigs.sigint)
	{
		if (!(check_fd(0) && ((ret = read(0, &buf, 4)) > 0)))
			continue ;
		if (buf[0] == 4)
		{
			if (!g_term.sigs.sigint)
			{
				close(0);
				ret = -1;
			}
			break;
		}
		ft_memcpy(thing.arr_form, buf, 4);
		if ((handle_controls(thing.long_form, buf)) < 1)
		{
			int cursor_pos = (g_term.conf.cursor[1] * g_term.conf.termsize[0]) + g_term.conf.cursor[0] - PROMPT_SIZE;
			tbuff_line_insert(g_term.curr_buff, buf, cursor_pos);
			reprint_buffer(g_term.curr_buff);
			termcap_reset_cursor(cursor_pos, ft_strlen(g_term.curr_buff->buff_str));
		}
		else if (thing.long_form == ENTER && !hd)
		{
			g_term.conf.cursor[0] = ft_strlen(hd ? HDPROMPT : PROMPT);
			g_term.conf.cursor[1] = 0;
			g_term.conf.curlines = 1;
			return (1);
		}
		else if (thing.long_form == ENTER)
		{
			ft_printf_fd(STDERR_FILENO, "enter hd\n");
			if (has_hd(tmp, stop))
				return (1);
			term_write(hd ? HDPROMPT : PROMPT, STDERR_FILENO, 1);
		}
		ft_memset(buf, 0, BUFF_SIZE + 1);
		if (g_term.sigs.sigint)
			break ;
	}
	return (ret);
}

void		shell_loop(void)
{
	int			quit;
	t_stats		stats;
	t_node		*tree;
	int			res;
	quit = 0;

	res = 0;
	g_term.buff = NULL;
	g_term.curr_buff = NULL;
	tree = NULL;
	read_rcfile();
	while (!quit)
	{
		g_term.sigs.sigint = false;
		g_term.conf.termsize[0] = g_window_size.ws_col;
		g_term.conf.termsize[1] = g_window_size.ws_row;
		if (g_term.sigs.sigint)
			continue ;
		if (!g_term.buff || (g_term.buff && g_term.buff->buff_str && *(g_term.buff->buff_str)))
			tbuff_new(&g_term.buff);
		g_term.curr_buff = g_term.buff;
		if (!(res = ft_readstdin_line(0, NULL)))
			continue ;
		if (res < 0)
			quit = 1;
		stats.f_d[0] = 0;
		stats.f_d[1] = 1;
		if (g_term.sigs.sigint || !g_term.curr_buff || !g_term.curr_buff->buff_str || (g_term.curr_buff->buff_str && !*(g_term.curr_buff->buff_str)))
			continue ;
		tree = lexer(g_term.curr_buff->buff_str);
		if (g_term.sigs.sigint)
		{
			clean_tree(tree);
			continue ;
		}
		recurse(tree, &stats);
		ft_printf_fd(STDERR_FILENO, "done recursing\n");
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
	shell_loop();
	reset_term();
	return (0);
}
