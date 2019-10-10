/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: calamber <calamber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/28 22:21:55 by alkozma           #+#    #+#             */
/*   Updated: 2019/10/09 10:31:13 by alkozma          ###   ########.fr       */
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
		exit (0);
	}
	//ft_putstr_fd(tgetstr("vi", &temp), STDERR_FILENO);
	ft_putstr_fd(tgetstr("ti", &temp), STDERR_FILENO);
	GET_SCREENSIZE;
	g_term.rows = 1;
}

void		reset_term(void)
{
	char	*temp;
	char	buf[150];

	temp = buf;
	tcsetattr(STDERR_FILENO, TCSANOW, &g_term.old_term);
	ft_putstr_fd(tgetstr("ve", &temp), STDERR_FILENO);
	ft_putstr_fd(tgetstr("te", &temp), STDERR_FILENO);
}

int			ft_readstdin_line(void)
{
	char	buf[BUFF_SIZE + 1];
	char	*tmp;
	int		ret;
	u_input	thing;

	g_term.conf.curlines = 1;
	ft_memset(buf, 0, BUFF_SIZE + 1);
	term_write(PROMPT, STDERR_FILENO, 1);
	g_term.conf.cursor[0] = ft_strlen(PROMPT);
	thing.long_form = 0;
	while ((ret = read(0, buf, 4)) >= 0)
	{
		ft_memcpy(thing.arr_form, buf, 4);
		if ((handle_controls(thing.long_form, buf, g_term.line_in)) < 1)
		{
			tmp = ft_strjoin(g_term.line_in, buf);
			if (g_term.line_in)
				free(g_term.line_in);
			g_term.line_in = tmp;
		}
		else if (thing.long_form == ENTER)
			return (1);
		ft_memset(buf, 0, BUFF_SIZE + 1);
	}
	return (0);
}

void		shell_loop(void)
{
	t_ast		*tree;
	int			quit;
	
	quit = 0;
	g_term.conf.termsize[0] = g_window_size.ws_col;
	g_term.conf.termsize[1] = g_window_size.ws_row;
	g_term.conf.cursor[0] = 0;
	g_term.conf.cursor[1] = 0;
	g_term.conf.curr_c = -1;
	g_term.line_in = NULL;
	tree = NULL;
	while (!quit)
	{
		if (!ft_readstdin_line())
			continue ;
		//else
		//	tree = parse_input();
		if (tree)
		{
			;
		}
		//quit = parse_tree(&tree);
		lexer();
	}
}

void		define_symbols(void)
{
	static char	*symbols[15] = { 
	"NONE",
	"WORD",
	"NUMBER",
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
int			main(int ac, char **av)
{
	//t_args	*args;

	if (ac || av)
	{
		;
	}

	if (!(validate_term()))
		return (0);
	if (!make_env(&g_term.env))
		return (0);
	init_term();
	set_sighandle();
	define_symbols();
	CLEAR_SCREEN;
	shell_loop();
	CLEAR_SCREEN;
	reset_term();
	return (0);
}
